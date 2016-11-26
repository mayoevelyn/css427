from flask import Flask, render_template, flash, request, send_from_directory
from wtforms import Form, TextField, validators, SelectField, RadioField
import pickle

#  App config
DEBUG = True
app = Flask(__name__)
app.config.from_object(__name__)
app.secret_key = 'A0A0818157573939'

# Form controls

class ToggleForm(Form):
    toggle = RadioField('Toggle Valve', choices=[(1, 'Open'), (0, 'Closed')], default=0)

class SensorsForm(Form):
    temperatureThreshold = TextField('Threshold:')
    humidityThreshold = TextField('Threshold:')
    brightnessThreshold = TextField('Threshold:')
    moistureThreshold = TextField('Threshold:')

class PreferencesForm(Form):
    sensorStartTime = SelectField('Start', choices=[(0, '12:00'), (1, '1:00'), (2, '2:00'), (3, '3:00'), (4, '4:00'), (5, '5:00'), (6, '6:00'), (7, '7:00'), (8, '8:00'), (9, '9:00'), (10, '10:00'), (11, '11:00')])
    sensorStartAMPM = SelectField('AM/PM', choices=[(0, 'AM'), (1, 'PM')])
    sensorEndTime = SelectField('End', choices=[(0, '12:00'), (1, '1:00'), (2, '2:00'), (3, '3:00'), (4, '4:00'), (5, '5:00'), (6, '6:00'), (7, '7:00'), (8, '8:00'), (9, '9:00'), (10, '10:00'), (11, '11:00')])
    sensorEndAMPM = SelectField('AM/PM', choices=[(0, 'AM'), (1, 'PM')])
    irrigationStartTime = SelectField('Schedule for', choices=[(0, '12:00'), (1, '1:00'), (2, '2:00'), (3, '3:00'), (4, '4:00'), (5, '5:00'), (6, '6:00'), (7, '7:00'), (8, '8:00'), (9, '9:00'), (10, '10:00'), (11, '11:00')])
    irrigationStartAMPM = SelectField('AM/PM', choices=[(0, 'AM'), (1, 'PM')])
    irrigationDuration = SelectField('Keep valve open for', choices=[(0.5, '30 minutes'), (1, '1 hour'), (1.5, '1.5 hours'), (2, '2 hours'), (2.5, '2.5 hours'), (3, '3 hours'), (3.5, '3.5 hours'), (4, '4 hours')])
    
# External file loads
fp = open ("averages.pkl")
currentAverages = pickle.load(fp)

fp = open("valveState.pkl")
valveState = pickle.load(fp)

fp = open("nextScheduled.pkl")
nextScheduled = pickle.load(fp)

fp = open("history.pkl")
history = pickle.load(fp)

fp = open("sensorData.pkl")
sensorData = pickle.load(fp)

@app.route("/toggleValve", methods=['GET', 'POST'])
def toggleValve():
    toggle_form = ToggleForm(request.form)

    fp = open("valveState.pkl")
    valveState = pickle.load(fp)

    fp = open("formValveSeqNum.pkl")
    formValveSeqNum = pickle.load(fp)
    
    if request.method == 'POST':        
        print "toggleValve function"        

        toggleChoice = request.form['toggle']
        formValveState = {}
        print toggleChoice

        if toggleChoice == '0':
            formValveState["ValveState"] = "Closed"
            
        if toggleChoice == '1':
            formValveState["ValveState"] = "Open"

        valveSeqNum = formValveSeqNum["SeqNum"]
        valveSeqNum = valveSeqNum + 1
        if valveSeqNum == 10:
            valveSeqNum = 0
        
        formValveSeqNum["SeqNum"] = valveSeqNum
        formValveState["SeqNum"] = valveSeqNum

        fp = open("formValveState.pkl", "w")
        pickle.dump(formValveState, fp)

        fp = open("formValveSeqNum.pkl", "w")
        pickle.dump(formValveSeqNum, fp)

    return render_template(
        'index.html',
        toggleForm=toggle_form,
        sensorsForm=SensorsForm(),
        preferencesForm=PreferencesForm(),
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )
        

@app.route("/sensors", methods=['GET', 'POST'])
def sensors():    
    sensors_form = SensorsForm(request.form)
    
    if request.method == 'POST':        
        print "sensors function"

        # Collect form data from form controls
        temperatureThreshold = request.form['temperatureThreshold']
        humidityThreshold = request.form['humidityThreshold']
        brightnessThreshold = request.form['brightnessThreshold']
        moistureThreshold = request.form['moistureThreshold']

        # Save form data to file
        formSensors = {}
        formSensors["TemperatureThreshold"] = temperatureThreshold
        formSensors["HumidityThreshold"] = humidityThreshold
        formSensors["BrightnessThreshold"] = brightnessThreshold
        formSensors["MoistureThreshold"] = moistureThreshold

        fp = open("formSensors.pkl", "w")
        pickle.dump(formSensors, fp)
        

    return render_template(
        'index.html',
        toggleForm=ToggleForm(),
        sensorsForm=sensors_form,
        preferencesForm=PreferencesForm(),
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )

@app.route("/preferences", methods=['GET', 'POST'])
def preferences():
    preferences_form = PreferencesForm(request.form)
    if request.method == 'POST':        
        print "preferences function"

        # Collect form data from form controls
        sensorStartTime = request.form['sensorStartTime']
        sensorStartAMPM = request.form['sensorStartAMPM']
        sensorEndTime = request.form['sensorEndTime']
        sensorEndAMPM = request.form['sensorEndAMPM']
        irrigationStartTime = request.form['irrigationStartTime']
        irrigationStartAMPM = request.form['irrigationStartAMPM']
        irrigationDuration = request.form['irrigationDuration']
        
        
        # Save form data to file
        formPreferences = {}
        formPreferences["SensorStartTime"] = sensorStartTime
        formPreferences["SensorStartAMPM"] = sensorStartAMPM
        formPreferences["SensorEndTime"] = sensorEndTime
        formPreferences["SensorEndAMPM"] = sensorEndAMPM
        formPreferences["IrrigationStartTime"] = irrigationStartTime
        formPreferences["IrrigationStartAMPM"] = irrigationStartAMPM
        formPreferences["IrrigationDuration"] = irrigationDuration

        fp = open("formPreferences.pkl", "w")
        pickle.dump(formPreferences, fp)
        

    return render_template(
        'index.html',
        toggleForm=ToggleForm(),
        sensorsForm=SensorsForm(),
        preferencesForm=preferences_form,
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )



@app.route("/", methods=['GET', 'POST'])
def index():
    return render_template(
        'index.html',
        sensorsForm=SensorsForm(),
        toggleForm=ToggleForm(),
        preferencesForm=PreferencesForm(),
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )
    

if __name__ == "__main__":
    app.run('0.0.0.0')
