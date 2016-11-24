from flask import Flask, render_template, flash, request
from wtforms import Form, TextField, TextAreaField, validators, StringField, SubmitField
#import pickle

#  App config
DEBUG = True
app = Flask(__name__)
app.config.from_object(__name__)
app.secret_key = 'A0A0818157573939'

class Login(Form):
    login_name = TextField('login')

class Register(Form):
    register_name = TextField('register')

@app.route('/login', methods=['POST'])
def login():
    l_form = Login(request.form)
    if request.method == 'POST':
        print request.form['login_name']
    return render_template('index.html', lform=l_form, rform=Register())    

@app.route('/register', methods=['POST'])
def register():
    r_form = Register(request.form)
    if request.method == 'POST':
        print request.form['register_name']
    return render_template('index.html', lform=Login(), rform=r_form)    

@app.route("/", methods=['POST'])
def index():    
    return render_template('index.html', lform=Login(), rform=Register())

if __name__ == "__main__":
    app.run('0.0.0.0')
