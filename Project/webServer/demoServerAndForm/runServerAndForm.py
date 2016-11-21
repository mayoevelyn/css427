from flask import Flask, render_template, flash, request
from wtforms import Form, TextField, TextAreaField, validators, StringField, SubmitField
import pickle

#  App config
DEBUG = True
app = Flask(__name__)
app.config.from_object(__name__)
app.config['SECRET_KEY'] = '7d441f27d441f27567d441f2b6176a'

class ReusableForm(Form):
    name = TextField('Name:', validators=[validators.required()])

@app.route("/", methods=['GET', 'POST'])
def renderPage():

    fp = open ("fromRas.pkl")
    fromRas = pickle.load(fp)


    form = ReusableForm(request.form)

    print form.errors
    if request.method == 'POST':
        name = request.form['name']
        print name

        if form.validate():
            # Save the comment here
            flash('hello ' + name)

            fromForm = {}
            fromForm["Name"] = name
            fp2 = open("fromForm.pkl", "w")
            pickle.dump(fromForm, fp2)
            
        else:
            flash('empty field not allowed')

            

    return render_template('serverAndForm.html', form=form, my_list=fromRas)

if __name__ == "__main__":
    app.run('0.0.0.0')
