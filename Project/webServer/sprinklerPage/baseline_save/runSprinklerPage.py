from flask import Flask, render_template, flash, request
from wtforms import Form, TextField, TextAreaField, validators, StringField, SubmitField
import pickle

#  App config
DEBUG = True
app = Flask(__name__)
app.config.from_object(__name__)
app.secret_key = 'A0A0818157573939'

formFields= {}

# Form data fields

class ToggleForm(Form):
    temperatureThreshold = TextField('Threshold:')

class SensorsForm(Form):
    temperatureThreshold = TextField('Threshold:')
    humidityThreshold = TextField('Threshold:')
    brightnessThreshold = TextField('Threshold:')
    moistureThreshold = TextField('Threshold:')

@app.route("/toggleValve", methods=['GET', 'POST'])
def toggleValve():
    toggle_form = ToggleForm()

    if request.method == 'POST':        
        print "toggleValve function"

    return render_template('index.html', toggleForm=toggle_form, sensorsForm=SensorsForm())

@app.route("/sensors", methods=['GET', 'POST'])
def sensors():
    # Form section
    sensors_form = SensorsForm(request.form)
    
    if request.method == 'POST':        
        print "sensors function"
        

    return render_template('index.html', toggleForm=ToggleForm(), sensorsForm=sensors_form)

@app.route("/", methods=['GET', 'POST'])
def index():  
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
    print "test"
    return render_template(
        'index.html',
        sensorsForm=SensorsForm(),
        toggleForm=ToggleForm(),        
        )
    

if __name__ == "__main__":
    app.run('0.0.0.0')
