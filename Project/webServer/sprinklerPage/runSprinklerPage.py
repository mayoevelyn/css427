from flask import Flask, render_template, flash, request, send_from_directory, redirect, url_for
from wtforms import Form, TextField, validators, SelectField, RadioField
import pickle

#  App config
DEBUG = True
app = Flask(__name__)
app.config.from_object(__name__)
app.secret_key = 'A0A0818157573939'



# Form controls

class ToggleForm(Form):
    toggle = RadioField('Valve Control', choices=[(1, 'Open'), (0, 'Closed'), (2, 'Toggle')], default=0)

class SensorsForm(Form):
    temperatureThreshold = TextField('New Threshold:')
    humidityThreshold = TextField('New Threshold:')
    brightnessThreshold = TextField('New Threshold:')
    moistureThreshold = TextField('New Threshold:')

class PreferencesForm(Form):
    sensorStartTime = SelectField('Start', choices=[(1, '1:00'), (2, '2:00'), (3, '3:00'), (4, '4:00'), (5, '5:00'), (6, '6:00'), (7, '7:00'), (8, '8:00'), (9, '9:00'), (10, '10:00'), (11, '11:00'), (12, '12:00')])
    sensorStartAMPM = SelectField('AM/PM', choices=[(0, 'AM'), (1, 'PM')])
    sensorEndTime = SelectField('End', choices=[(1, '1:00'), (2, '2:00'), (3, '3:00'), (4, '4:00'), (5, '5:00'), (6, '6:00'), (7, '7:00'), (8, '8:00'), (9, '9:00'), (10, '10:00'), (11, '11:00'), (12, '12:00')])
    sensorEndAMPM = SelectField('AM/PM', choices=[(0, 'AM'), (1, 'PM')])
    irrigationStartTime = SelectField('Schedule for', choices=[(1, '1:00'), (2, '2:00'), (3, '3:00'), (4, '4:00'), (5, '5:00'), (6, '6:00'), (7, '7:00'), (8, '8:00'), (9, '9:00'), (10, '10:00'), (11, '11:00'), (12, '12:00')])
    irrigationStartAMPM = SelectField('AM/PM', choices=[(0, 'AM'), (1, 'PM')])
    irrigationDuration = SelectField('Keep valve open for', choices=[(0.5, '30 minutes'), (1, '1 hour'), (1.5, '1.5 hours'), (2, '2 hours'), (2.5, '2.5 hours'), (3, '3 hours'), (3.5, '3.5 hours'), (4, '4 hours')])

class ForceSensorUpdatesForm(Form):
    hiddenField = ""

@app.route("/toggleValve", methods=['GET', 'POST'])
def toggleValve():
    toggle_form = ToggleForm(request.form)

    # Load the previous valve state
    fp = open("cmdStationData/valveState.pkl")
    valveState = pickle.load(fp)

    # Load the previous sequence number
    fp = open("sequenceNumbers/formValveSeqNum.pkl")
    formValveSeqNum = pickle.load(fp)
    
    if request.method == 'POST':        
        print "toggleValve function"        

        # Read the value of the form radio button
        toggleChoice = request.form['toggle']
        formValveState = {}
        print toggleChoice

        # Package the radio button choice
        if toggleChoice == '0':
            formValveState["ValveState"] = "Closed"
            
        if toggleChoice == '1':
            formValveState["ValveState"] = "Open"

        if toggleChoice == '2':
            formValveState["ValveState"] = "Toggle"

        # Increment the sequence number
        valveSeqNum = formValveSeqNum["SeqNum"]
        valveSeqNum = valveSeqNum + 1
        if valveSeqNum == 10:
            valveSeqNum = 0
        
        formValveSeqNum["SeqNum"] = valveSeqNum
        formValveState["SeqNum"] = valveSeqNum

        # Save sequence number to file
        fp = open("sequenceNumbers/formValveSeqNum.pkl", "w")
        pickle.dump(formValveSeqNum, fp)

        # Save form data to file
        fp = open("formData/formValveState.pkl", "w")
        pickle.dump(formValveState, fp)

    return redirect(url_for('index'))
"""
    # External file reloads
    fp = open ("cmdStationData/averages.pkl")
    currentAverages = pickle.load(fp)
    fp = open("cmdStationData/valveState.pkl")
    valveState = pickle.load(fp)
    fp = open("cmdStationData/nextScheduled.pkl")
    nextScheduled = pickle.load(fp)
    fp = open("cmdStationData/history.pkl")
    history = pickle.load(fp)
    fp = open("cmdStationData/sensorData.pkl")
    sensorData = pickle.load(fp)

    return render_template(
        'index.html',
        toggleForm=toggle_form,
        sensorsForm=SensorsForm(),
        preferencesForm=PreferencesForm(),
        forceSensorUpdatesForm=ForceSensorUpdatesForm(),
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )
        """

@app.route("/sensors", methods=['GET', 'POST'])
def sensors():    
    sensors_form = SensorsForm(request.form)

    # Load the previous sequence number
    fp = open("sequenceNumbers/formSensorsSeqNum.pkl")
    formSensorsSeqNum = pickle.load(fp)

    if request.method == 'POST':        
        print "sensors function"

        # Collect form data from controls
        temperatureThreshold = request.form['temperatureThreshold']
        humidityThreshold = request.form['humidityThreshold']
        brightnessThreshold = request.form['brightnessThreshold']
        moistureThreshold = request.form['moistureThreshold']

        # Package sensor readings to be written
        formSensors = {}
        formSensors["TemperatureThreshold"] = temperatureThreshold
        formSensors["HumidityThreshold"] = humidityThreshold
        formSensors["BrightnessThreshold"] = brightnessThreshold
        formSensors["MoistureThreshold"] = moistureThreshold        

        # Increment the sequence number
        sensorsSeqNum = formSensorsSeqNum["SeqNum"]
        sensorsSeqNum = sensorsSeqNum + 1
        if sensorsSeqNum == 10:
            sensorsSeqNum = 0
        
        formSensorsSeqNum["SeqNum"] = sensorsSeqNum
        formSensors["SeqNum"] = sensorsSeqNum

        # Save sequence number to file
        fp = open("sequenceNumbers/formSensorsSeqNum.pkl", "w")
        pickle.dump(formSensorsSeqNum, fp)
        
        # Save form data to file
        fp = open("formData/formSensors.pkl", "w")
        pickle.dump(formSensors, fp)

    return redirect(url_for('index'))

    """# External file reloads
    fp = open ("cmdStationData/averages.pkl")
    currentAverages = pickle.load(fp)
    fp = open("cmdStationData/valveState.pkl")
    valveState = pickle.load(fp)
    fp = open("cmdStationData/nextScheduled.pkl")
    nextScheduled = pickle.load(fp)
    fp = open("cmdStationData/history.pkl")
    history = pickle.load(fp)
    fp = open("cmdStationData/sensorData.pkl")
    sensorData = pickle.load(fp)    

    return render_template(
        'index.html',
        toggleForm=ToggleForm(),
        sensorsForm=sensors_form,
        preferencesForm=PreferencesForm(),
        forceSensorUpdatesForm=ForceSensorUpdatesForm(),
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )
        """

@app.route("/preferences", methods=['GET', 'POST'])
def preferences():
    preferences_form = PreferencesForm(request.form)

    # Load the previous sequence number
    fp = open("sequenceNumbers/formPreferencesSeqNum.pkl")
    formPreferencesSeqNum = pickle.load(fp)
    
    if request.method == 'POST':        
        print "preferences function"

        # Collect form data from controls
        sensorStartTime = request.form['sensorStartTime']
        sensorStartAMPM = request.form['sensorStartAMPM']
        sensorEndTime = request.form['sensorEndTime']
        sensorEndAMPM = request.form['sensorEndAMPM']
        irrigationStartTime = request.form['irrigationStartTime']
        irrigationStartAMPM = request.form['irrigationStartAMPM']
        irrigationDuration = request.form['irrigationDuration']        
        
        # Package preferences to be written
        formPreferences = {}
        formPreferences["SensorStartTime"] = sensorStartTime
        formPreferences["SensorStartAMPM"] = sensorStartAMPM
        formPreferences["SensorEndTime"] = sensorEndTime
        formPreferences["SensorEndAMPM"] = sensorEndAMPM
        formPreferences["IrrigationStartTime"] = irrigationStartTime
        formPreferences["IrrigationStartAMPM"] = irrigationStartAMPM
        formPreferences["IrrigationDuration"] = irrigationDuration

        # Increment the sequence number
        preferencesSeqNum = formPreferencesSeqNum["SeqNum"]
        preferencesSeqNum = preferencesSeqNum + 1
        if preferencesSeqNum == 10:
            preferencesSeqNum = 0
        
        formPreferencesSeqNum["SeqNum"] = preferencesSeqNum
        formPreferences["SeqNum"] = preferencesSeqNum

        # Save sequence number to file
        fp = open("sequenceNumbers/formPreferencesSeqNum.pkl", "w")
        pickle.dump(formPreferencesSeqNum, fp)

        # Save form data to file
        fp = open("formData/formPreferences.pkl", "w")
        pickle.dump(formPreferences, fp)        

    return redirect(url_for('index'))

    """# External file reloads
    fp = open ("cmdStationData/averages.pkl")
    currentAverages = pickle.load(fp)
    fp = open("cmdStationData/valveState.pkl")
    valveState = pickle.load(fp)
    fp = open("cmdStationData/nextScheduled.pkl")
    nextScheduled = pickle.load(fp)
    fp = open("cmdStationData/history.pkl")
    history = pickle.load(fp)
    fp = open("cmdStationData/sensorData.pkl")
    sensorData = pickle.load(fp)

    return render_template(
        'index.html',
        toggleForm=ToggleForm(),
        sensorsForm=SensorsForm(),
        preferencesForm=preferences_form,
        forceSensorUpdatesForm=ForceSensorUpdatesForm(),
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )
        """

@app.route("/forceSensorUpdates", methods=['GET', 'POST'])
def forceSensorUpdates():
    forceSensorUpdates_form = ForceSensorUpdatesForm(request.form)

    # Load the previous sequence number
    fp = open("sequenceNumbers/formForceSensorUpdatesSeqNum.pkl")
    formForceSensorUpdatesSeqNum = pickle.load(fp)
        
    if request.method == 'POST':        
        print "force sensor updates function"

        # Increment the sequence number
        forceSensorUpdatesSeqNum = formForceSensorUpdatesSeqNum["SeqNum"]
        forceSensorUpdatesSeqNum = forceSensorUpdatesSeqNum + 1
        
        if forceSensorUpdatesSeqNum == 10:
            forceSensorUpdatesSeqNum = 0
        
        formForceSensorUpdatesSeqNum["SeqNum"] = forceSensorUpdatesSeqNum        

        # Save sequence number to file
        fp = open("sequenceNumbers/formForceSensorUpdatesSeqNum.pkl", "w")
        pickle.dump(formForceSensorUpdatesSeqNum, fp)

    return redirect(url_for('index'))
    
    """# External file reloads
    fp = open ("cmdStationData/averages.pkl")
    currentAverages = pickle.load(fp)
    fp = open("cmdStationData/valveState.pkl")
    valveState = pickle.load(fp)
    fp = open("cmdStationData/nextScheduled.pkl")
    nextScheduled = pickle.load(fp)
    fp = open("cmdStationData/history.pkl")
    history = pickle.load(fp)
    fp = open("cmdStationData/sensorData.pkl")
    sensorData = pickle.load(fp)

    return render_template(
        'index.html',
        toggleForm=ToggleForm(),
        sensorsForm=SensorsForm(),
        preferencesForm=PreferencesForm(),
        forceSensorUpdatesForm=forceSensorUpdates_form,
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )
        """

@app.route("/", methods=['GET', 'POST'])
def index():
    # External file reloads
    fp = open ("cmdStationData/averages.pkl")
    currentAverages = pickle.load(fp)
    fp = open("cmdStationData/valveState.pkl")
    valveState = pickle.load(fp)
    fp = open("cmdStationData/nextScheduled.pkl")
    nextScheduled = pickle.load(fp)
    fp = open("cmdStationData/history.pkl")
    history = pickle.load(fp)
    fp = open("cmdStationData/sensorData.pkl")
    sensorData = pickle.load(fp)

    return render_template(
        'index.html',
        sensorsForm=SensorsForm(),
        toggleForm=ToggleForm(),
        preferencesForm=PreferencesForm(),
        forceSensorUpdatesForm=ForceSensorUpdatesForm(),
        currentAverages=currentAverages,
        valveState=valveState,
        nextScheduled=nextScheduled,
        history=history,
        sensorData=sensorData
        )
    

if __name__ == "__main__":
    # Init sequence numbers

    # Valve
    formValveSeqNum = {}
    formValveSeqNum["SeqNum"] = 0
    fp = open("sequenceNumbers/formValveSeqNum.pkl", "w")
    pickle.dump(formValveSeqNum, fp)
    fp.close()

    fp = open("formData/formValveState.pkl")
    formValveState = pickle.load(fp)
    fp.close()

    formValveState["SeqNum"] = 0
    fp = open("formData/formValveState.pkl", "w")
    pickle.dump(formValveState, fp)
    fp.close()

    # Sensors
    formSensorsSeqNum = {}
    formSensorsSeqNum["SeqNum"] = 0
    fp = open("sequenceNumbers/formSensorsSeqNum.pkl", "w")
    pickle.dump(formSensorsSeqNum, fp)
    fp.close()

    fp = open("formData/formValveState.pkl")
    formSensors = pickle.load(fp)
    fp.close()

    formSensors["SeqNum"] = 0
    fp = open("formData/formSensors.pkl", "w")
    pickle.dump(formSensors, fp)
    fp.close()

    # Preferences
    formPreferencesSeqNum = {}
    formPreferencesSeqNum["SeqNum"] = 0
    fp = open("sequenceNumbers/formPreferencesSeqNum.pkl", "w")
    pickle.dump(formPreferencesSeqNum, fp)
    fp.close()

    fp = open("formData/formPreferences.pkl")
    formPreferences = pickle.load(fp)
    fp.close()

    formPreferences["SeqNum"] = 0
    fp = open("formData/formPreferences.pkl", "w")
    pickle.dump(formPreferences, fp)
    fp.close()

    # Force Sensor Updates
    formForceSensorUpdatesSeqNum = {}
    formForceSensorUpdatesSeqNum["SeqNum"] = 0
    fp = open("sequenceNumbers/formForceSensorUpdatesSeqNum.pkl", "w")
    pickle.dump(formForceSensorUpdatesSeqNum, fp)
    fp.close()

    # Start the server
    app.run('0.0.0.0')
