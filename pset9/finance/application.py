import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    query = "SELECT B.symbol symbol, (B.shares - COALESCE(S.shares, 0)) shares \
        FROM (SELECT symbol, SUM(shares) shares FROM transactions WHERE user_id = ? AND type = 'buy' GROUP BY symbol) AS B \
        LEFT JOIN (SELECT symbol, SUM(shares) shares FROM transactions WHERE user_id = ? AND type = 'sell' GROUP BY symbol) AS S \
        ON B.symbol = S.symbol"
    stocks = db.execute(query, session["user_id"], session["user_id"])

    cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]['cash']

    rows = []
    balance = cash
    for stock in stocks:
        if stock["shares"] <= 0:
            continue

        quote = lookup(stock["symbol"])

        row = {
            "symbol": quote["symbol"],
            "name": quote["name"],
            "shares": stock["shares"],
            "price": quote["price"],
            "total": stock["shares"] * quote["price"]
        }
        rows.append(row)

        balance += row["shares"] * row["price"]

    data = {"rows": rows, "cash": cash, "balance": balance}

    return render_template("index.html", data=data)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        data = lookup(symbol)

        if not data:
            return apology("The symbol does not exist", 400)
        if not shares.isdigit():
            return apology("shares must be positive integer", 400)
        else:
            shares = int(float(shares))

        price = data["price"]
        row = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        if row[0]["cash"] < price * shares:
            return apology("not enough cash in your account", 400)
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", row[0]["cash"] - price * shares, session["user_id"])
            db.execute("INSERT INTO transactions (symbol, price, shares, type, user_id) VALUES (?, ?, ?, ?, ?)", symbol, price, shares, "buy", session["user_id"])

            return index()

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    data = db.execute("SELECT symbol, shares shares, price, time, type FROM transactions WHERE user_id = ?", session["user_id"])

    return render_template("history.html", data=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        data = lookup(symbol)

        if not data:
            return apology("Invalid symbol", 400)
        else:
            return render_template("quoted.html", data=data)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # input data sanity check
        username = request.form.get("username")
        password = request.form.get("password")
        row = db.execute("SELECT * FROM users WHERE username = ?", username)
        if not username:
            return apology("must provide username", 400)
        elif len(row) != 0:
            return apology("username already exists", 400)
        elif not password:
            return apology("must provide password", 400)
        elif password != request.form.get("confirmation"):
            return apology("type the same password twice to confirm", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares2sell = int(float(request.form.get("shares")))

        if not symbol:
            return apology("missing symbol", 400)
        elif not shares2sell:
            return apology("missing shares", 400)

        query = "SELECT B.symbol symbol, (B.shares - COALESCE(S.shares, 0)) shares \
            FROM (SELECT symbol, SUM(shares) shares FROM transactions WHERE user_id = ? AND type = 'buy' GROUP BY symbol) AS B \
            LEFT JOIN (SELECT symbol, SUM(shares) shares FROM transactions WHERE user_id = ? AND type = 'sell' GROUP BY symbol) AS S \
            ON B.symbol = S.symbol \
            WHERE B.symbol = ?"
        data = db.execute(query, session["user_id"], session["user_id"], symbol)

        if (len(data) == 0) or (data[0]["shares"] < shares2sell):
            return apology("not enough shares", 400)

        price = lookup(data[0]["symbol"])["price"]
        cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + shares2sell * price, session["user_id"])
        db.execute("INSERT INTO transactions (symbol, price, shares, type, user_id) VALUES (?, ?, ?, ?, ?)", symbol, price, shares2sell, "sell", session["user_id"])

        return index()

    else:
        query = "SELECT B.symbol symbol, (B.shares - COALESCE(S.shares, 0)) shares \
            FROM (SELECT symbol, SUM(shares) shares FROM transactions WHERE user_id = ? AND type = 'buy' GROUP BY symbol) AS B \
            LEFT JOIN (SELECT symbol, SUM(shares) shares FROM transactions WHERE user_id = ? AND type = 'sell' GROUP BY symbol) AS S \
            ON B.symbol = S.symbol"
        data = db.execute(query, session["user_id"], session["user_id"])

        return render_template("sell.html", data=data)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
