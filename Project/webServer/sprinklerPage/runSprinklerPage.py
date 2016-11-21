from flask import Flask, render_template, flash, request
from wtforms import Form, TextField, TextAreaField, validators, StringField, SubmitField
import pickle

#  App config
DEBUG = True
app = Flask(__name__)
app.config.from_object(__name__)
app.secret_key = 'A0A0818157573939'

# Form data fields
class ReusableForm(Form):
    temperatureThreshold = TextField('Threshold:', validators=[validators.required()])
    humidityThreshold = TextField('Threshold:', validators=[validators.required()])
    brightnessThreshold = TextField('Threshold:', validators=[validators.required()])
    moistureThreshold = TextField('Threshold:', validators=[validators.required()])

@app.route("/", methods=['GET', 'POST'])
def renderPage():    

    # Form section
    form = ReusableForm(request.form)

    print form.errors
    if request.method == 'POST':

        # Collect form data from form controls
        temperatureThreshold = request.form['temperatureThreshold']
        humidityThreshold = request.form['humidityThreshold']
        brightnessThreshold = request.form['brightnessThreshold']
        moistureThreshold = request.form['moistureThreshold']        

        if form.validate():

            # Save form data to file
            formFields= {}
            formFields["TemperatureThreshold"] = temperatureThreshold
            formFields["HumidityThreshold"] = humidityThreshold
            formFields["BrightnessThreshold"] = brightnessThreshold
            formFields["MoistureThreshold"] = moistureThreshold

            fp = open("formFields.pkl", "w")
            pickle.dump(formFields, fp)
            
        else:
            flash('empty fields not allowed')            

    # External file loads 
    fp = open ("averages.pkl")
    currentAverages = pickle.load(fp)

    fp = open("valveState.pkl")
    valveState = pickle.load(fp)

    fp = open("nextScheduled.pkl")
    nextScheduled = pickle.load(fp)

    fp = open("history.pkl")
    history = pickle.load(fp)

    fp = open("sensors.pkl")
    sensors = pickle.load(fp)

    return render_template(
        'sprinklerPage.html',
        form=form,
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensors=sensors)

if __name__ == "__main__":
    app.run('0.0.0.0')
