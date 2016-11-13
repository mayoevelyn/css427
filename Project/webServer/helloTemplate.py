from flask import Flask
from flask import render_template

app = Flask(__name__)

@app.route("/")

def mypysite(name=None):
    return render_template('helloPage.html')

if __name__ == "__main__":
    app.run('0.0.0.0')
