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
    temperatureThreshold = TextField('Threshold:')
    humidityThreshold = TextField('Threshold:')
    brightnessThreshold = TextField('Threshold:')
    moistureThreshold = TextField('Threshold:')

@app.route("/", methods=['GET', 'POST'])
def renderPage():    

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

    # Form section
    form = ReusableForm(request.form)

    print form.errors
    if request.method == 'POST':

        formFields= {}

        # Toggle valve
        if request.form.get("action") == "valve":
            if valveState["ValveState"] == "Open":
                formFields["ValveState"] = "Closed"
            else:
                formFields["ValveState"] = "Open"
            

        # Collect form data from form controls
        temperatureThreshold = request.form['temperatureThreshold']
        humidityThreshold = request.form['humidityThreshold']
        brightnessThreshold = request.form['brightnessThreshold']
        moistureThreshold = request.form['moistureThreshold']        

        

        # Save form data to file
            
        formFields["TemperatureThreshold"] = temperatureThreshold
        formFields["HumidityThreshold"] = humidityThreshold
        formFields["BrightnessThreshold"] = brightnessThreshold
        formFields["MoistureThreshold"] = moistureThreshold
        fp = open("formFields.pkl", "w")
        pickle.dump(formFields, fp)

        if request.form.get("action") == "Submit":
            print "Submit"
        if request.form.get("action") == "SomethingElse":
            print "Something Else"
            
                

    

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
