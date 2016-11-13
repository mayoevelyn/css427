from flask import Flask
import pickle

app = Flask(__name__)

@app.route("/")

def hello():
    fp = open ("shared.pkl")
    shared = pickle.load(fp)
    retval = str(shared["Foo"])
    return retval

if __name__ == "__main__":
    app.run('0.0.0.0')
