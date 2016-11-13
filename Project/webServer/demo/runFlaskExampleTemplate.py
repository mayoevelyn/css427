from flask import Flask, render_template
import pickle
app = Flask(__name__)


@app.route("/")
def template_test():
    fp = open ("shared.pkl")
    shared = pickle.load(fp)
    
    return render_template('flaskExampleTemplate.html', my_list=shared)


if __name__ == '__main__':
    app.run('0.0.0.0')


