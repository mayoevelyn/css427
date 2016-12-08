import serial
import pickle
from xbee import ZigBee
from datetime import datetime
from collections import defaultdict
import time
from threading import Thread


sensorAverageCount = 0
lightAverage = 0
temperatureAverage = 0
humidityAverage = 0
moistureAverage = 0

valveSeqNum = 1
sensorsSeqNum = 1
preferencesSeqNum = 1
forceSensorUpdatesSeqNum = 1

def forwardCallback(indata):    
    onDataReceived(indata)    

serial_port = serial.Serial('/dev/ttyUSB0', 9600)
xbee = ZigBee(serial_port, callback=forwardCallback)


def sendToController(outdata):
    global xbee
    DEST_ADDR_LONG = "\x00\x13\xA2\x00\x40\xE3\xCD\x1E"
    DEST_ADDR = "\xFF\xFE"

    # Send packet
    print "send to controller"
    print outdata
    xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data=outdata)            
        
    # Wait for ack
    #ack = xbee.wait_read_frame()
    
    print "send " + outdata + " to controller"
    

def updateValveReading(zone, valveState):
    # update valve reading, for now ignore zone
    newValveState = {}

    if valveState == "0":
        newValveState["ValveState"] = "Closed"
    else:
        newValveState["ValveState"] = "Open"

    fp = open("../webServer/sprinklerPage/cmdStationData/valveState.pkl", "w")
    pickle.dump(newValveState, fp)
    fp.close()
    print "zone " + zone + " valveState " + valveState

def isHot(temperature, threshold):
    # helper for setSchedule
    if temperature > threshold:
        return True
    else:
        return False

def isHumid(humidity, threshold):
    # helper for setSchedule
    if humidity > threshold:
        return True
    else:
        return False

def isBright(brightness, threshold):
    # helper for setSchedule
    if brightness > threshold:
        return True
    else:
        return False

def isMoist(moisture, threshold):
    # helper for setSchedule
    if moisture < threshold:
        return True
    else:
        return False

def recordHistory(wasIrrigated, scheduleHour, scheduleMinute):
    fp = open("../webServer/sprinklerPage/cmdStationData/history.pkl")
    history = pickle.load(fp)
    fp.close()

    dateAndTime = datetime.now().strftime("%Y-%m-%d %H:%M")
    history[str(dateAndTime)]["Date"] = str(dateAndTime)

    history[str(dateAndTime)]["Irrigation"] = wasIrrigated
    fp = open("../webServer/sprinklerPage/cmdStationData/history.pkl", "w")
    pickle.dump(history, fp)
    fp.close()

    if wasIrrigated == "SKIPPED":
        return

    nextScheduled = {}
    nextScheduled["Time"] = str(dateAndTime)
    
    fp = open("../webServer/sprinklerPage/cmdStationData/nextScheduled.pkl", "w")
    pickle.dump(nextScheduled, fp)
    fp.close()


def setSchedule(zone, lightAverage, temperatureAverage, humidityAverage, moistureAverage):
    # determine if the schedule must be set and set it if needed, for now ignore zone
    fp = open("../webServer/sprinklerPage/cmdStationData/sensorData.pkl")
    sensorData = pickle.load(fp)
    tempThreshold = int(sensorData["Temperature"]["Threshold"])
    humidityThreshold = int(sensorData["Humidity"]["Threshold"])
    lightThreshold = int(sensorData["Brightness"]["Threshold"])
    moistureThreshold = int(sensorData["Moisture"]["Threshold"])

    hot = isHot(temperatureAverage, tempThreshold)
    humid = isHumid(humidityAverage, humidityThreshold)
    bright = isBright(lightAverage, lightThreshold)
    moist = isMoist(moistureAverage, moistureThreshold)

    currentHour = int(datetime.now().strftime("%H"))
    currentMinute = int(datetime.now().strftime("%M"))
    
    # for demo only - set schedule to one minute in advance
    scheduleMinute = 0
    scheduleHour = 0
    if currentMinute < 59:
        scheduleHour = currentHour
        scheduleMinute = currentMinute + 1
        
    else:
        scheduleHour = currentHour + 1
        scheduleMinute = 0
            
    # for demo only, duration is one minute

    # determine whether to schedule irrigation, ignore zone for now
    if  not humid and not moist:
        # water: not humid and not moist
        schedule = "25," + str(zone) + "," + str(scheduleHour) + "," + str(scheduleMinute) + "," + "1"
        sendToController(schedule)
        print "not humid and not moist"
        
        recordHistory("SCHEDULED", scheduleHour, scheduleMinute)        
        return

    if bright and not moist:
        # water: sunny and not moist
        schedule = "25," + str(zone) + "," + str(scheduleHour) + "," + str(scheduleMinute) + "," + "1"
        sendToController(schedule)
        print "sunny and not moist"

        recordHistory("SCHEDULED", scheduleHour, scheduleMinute) 
        return

    if hot and not moist:
        # water: hot and not moist
        schedule = "25," + str(zone) + "," + str(scheduleHour) + "," + str(scheduleMinute) + "," + "1"
        sendToController(schedule)
        print "hot and not moist"

        recordHistory("SCHEDULED", scheduleHour, scheduleMinute) 
        return
    
    if hot and not humid and bright:
        # water: hot and not humid and sunny
        schedule = "25," + str(zone) + "," + str(scheduleHour) + "," + str(scheduleMinute) + "," + "1"
        sendToController(schedule)
        print "hot and not humid and sunny"

        recordHistory("SCHEDULED", scheduleHour, scheduleMinute) 
        return

    print("don't water:\nhot: " + str(hot) + "\nhumid: " + str(humid) + "\nbright: " + str(bright) + "\nmoist: " + str(moist))
    recordHistory("SKIPPED", -1, -1) 
    

def updateAverages(zone, light, temperature, humidity, moisture):
    # update sensor averages, for now ignore zone
    global sensorAverageCount
    global lightAverage
    global temperatureAverage
    global humidityAverage
    global moistureAverage

    sensorAverageCount = sensorAverageCount + 1    
    lightAverage = lightAverage + light
    temperatureAverage = temperatureAverage + temperature
    humidityAverage = humidityAverage + humidity
    moistureAverage = moistureAverage + moisture

    if sensorAverageCount == 4:
        sensorAverageCount = 0
        lightAverage = int(lightAverage / 4)
        temperatureAverage = int(temperatureAverage / 4)
        humidityAverage = int(humidityAverage / 4)
        moistureAverage = int(moistureAverage / 4)

        print "lightAverage " + str(lightAverage)
        print "temperatureAverage " + str(temperatureAverage)
        print "humidityAverage " + str(humidityAverage)
        print "moistureAverage " + str(moistureAverage)

        currentAverages = {
            "Temperature":str(temperatureAverage) + " C",
            "Humidity":str(humidityAverage) + " %",
            "Brightness":str(lightAverage) + " Lux",
            "Moisture":str(moistureAverage) + " Ohms"}

        fp = open("../webServer/sprinklerPage/cmdStationData/averages.pkl", "w")
        pickle.dump(currentAverages, fp)
        fp.close()
        
        setSchedule(zone, lightAverage, temperatureAverage, humidityAverage, moistureAverage)

        sensorAverageCount = 0
        lightAverage = 0
        temperatureAverage = 0
        humidityAverage = 0
        moistureAverage = 0
    

def updateSensorReadings(zone, light, temperature, humidity, moisture):
    # update sensor readings
    #print "trace updateSensorReadings"
    fp = open("../webServer/sprinklerPage/cmdStationData/sensorData.pkl")
    sensorData = pickle.load(fp)    

    newSensorData = defaultdict(dict)
    
    newSensorData["Temperature"]["Reading"] = int(temperature)    
    newSensorData["Temperature"]["Threshold"] = sensorData["Temperature"]["Threshold"]    
    newSensorData["Humidity"]["Reading"] = int(humidity)
    newSensorData["Humidity"]["Threshold"] = sensorData["Humidity"]["Threshold"]
    newSensorData["Brightness"]["Reading"] = int(light)
    newSensorData["Brightness"]["Threshold"] = sensorData["Brightness"]["Threshold"]
    newSensorData["Moisture"]["Reading"] = int(moisture)
    newSensorData["Moisture"]["Threshold"] = sensorData["Moisture"]["Threshold"]
   
    fp = open("../webServer/sprinklerPage/cmdStationData/sensorData.pkl", "w")
    pickle.dump(newSensorData, fp)
    fp.close()

    updateAverages(zone, light, temperature, humidity, moisture)

def interpretData(splitData):
    code = splitData[0]

    if code == "48":
        # update command station valve data for zone Z: 0 is closed, 1 is open
        zone = splitData[1]
        valveState = splitData[2]
        updateValveReading(zone, valveState)

    if code == "50":
        # update data for sensor readings for zone Z: Light, Temp, Hum, Moist
        #print "trace code == 50"
        zone = splitData[1]
        light = int(splitData[2])
        temperature = int(splitData[3])
        humidity = int(splitData[4])
        moisture = int(splitData[5])
        updateSensorReadings(zone, light, temperature, humidity, moisture)   

    
def checkWebServer():
    time.sleep(0.25)

    ############################################
    # Valve toggle
    
    try:
        global valveSeqNum
        fp = open("../webServer/sprinklerPage/formData/formValveState.pkl")
        formValveState = pickle.load(fp)
    
        if valveSeqNum <= formValveState["SeqNum"]:            
            # do work here
            valveState = formValveState["ValveState"]
            
            if valveState == "Open":
                print("Form Valve State: Open")
                sendToController("17,1") # hard code zone 1 for now

            if valveState == "Closed":
                print("Form Valve State: Closed")
                sendToController("18,1") # hard code zone 1 for now

            if valveState == "Toggle":
                print("Form Valve State: Toggle")
                sendToController("19,1") # hard code zone 1 for now

            #increment sequence number
            valveSeqNum = valveSeqNum + 1            

        # wrap around sequence number
        if formValveState["SeqNum"] == 0 and valveSeqNum == 10:
            valveSeqNum = 0           
            
    except:
        print "valve exception"

    ############################################
    # Sensors
    try:
        global sensorsSeqNum
        fp = open("../webServer/sprinklerPage/formData/formSensors.pkl")
        formSensors = pickle.load(fp)

        fp = open("../webServer/sprinklerPage/cmdStationData/sensorData.pkl")
        sensorData = pickle.load(fp)
    
        if sensorsSeqNum <= formSensors["SeqNum"]:
            # do work here
            print("Sensor Thresholds")
            
            try:
                tThresh = int(formSensors["TemperatureThreshold"])
                sensorData["Temperature"]["Threshold"] = tThresh
                print("Temperature Threshold: " + str(tThresh))
            except:
                pass

            try:
                hThresh = int(formSensors["HumidityThreshold"])
                sensorData["Humidity"]["Threshold"] = hThresh
                print("Humidity Threshold: " + str(hThresh))
            except:
                pass

            try:
                bThresh = int(formSensors["BrightnessThreshold"])
                sensorData["Brightness"]["Threshold"] = bThresh
                print("Brightness Threshold: " + str(bThresh))
            except:
                pass

            try:
                mThresh = int(formSensors["MoistureThreshold"])
                sensorData["Moisture"]["Threshold"] = mThresh
                print("Moisture Threshold: " + str(mThresh))
            except:
                pass

            fp = open("../webServer/sprinklerPage/cmdStationData/sensorData.pkl", "w")
            pickle.dump(sensorData, fp)
            fp.close()

            # increment sequence number
            sensorsSeqNum = sensorsSeqNum + 1

        # wrap around sequence number
        if formSensors["SeqNum"] == 0 and sensorsSeqNum == 10:
            sensorsSeqNum = 0                 
            
    except:
        print "sensors exception"

    ############################################
    # Preferences
    try:
        global preferencesSeqNum
        fp = open("../webServer/sprinklerPage/formData/formPreferences.pkl")
        formPreferences = pickle.load(fp)
    
        if preferencesSeqNum <= formPreferences["SeqNum"]:
            # do work here
            print("Preferences")

            print("Sensors start: " + str(formPreferences["SensorStartTime"]))
            if formPreferences["SensorStartAMPM"] == "0":
                  print "AM"
            else:
                  print "PM"

            print("Sensors end: " + str(formPreferences["SensorEndTime"]))
            if formPreferences["SensorEndAMPM"] == "0":
                  print "AM"
            else:
                  print "PM"

            print("Irrigation time: " + str(formPreferences["IrrigationStartTime"]))
            if formPreferences["IrrigationStartAMPM"] == "0":
                  print "AM"
            else:
                  print "PM"

            print("Irrigation duration: " + str(formPreferences["IrrigationDuration"]))            

            # increment sequence number
            preferencesSeqNum = preferencesSeqNum + 1

        # wrap around sequence number
        if formPreferences["SeqNum"] == 0 and preferencesSeqNum == 10:
            preferencesSeqNum = 0           
            
    except:
        print "preferences exception"

    ############################################
    # Force Sensor Updates
    try:
        global forceSensorUpdatesSeqNum
        fp = open("../webServer/sprinklerPage/sequenceNumbers/formForceSensorUpdatesSeqNum.pkl")
        formForceSensorUpdatesSeqNum = pickle.load(fp)

        if forceSensorUpdatesSeqNum <= formForceSensorUpdatesSeqNum["SeqNum"]:
            # do work here
            print "force sensor updates"
            sendToController("22,1") # hard coding zone 1 for now

            # increment sequence number
            forceSensorUpdatesSeqNum = forceSensorUpdatesSeqNum + 1

        # wrap around sequence number
        if formForceSensorUpdatesSeqNum["SeqNum"] == 0 and forceSensorUpdatesSeqNum == 10:
            forceSensorUpdatesSeqNum = 0

    except:
        print "force sensor updates exception"
    
def onDataReceived(indata):
    # Extract payload
    rfdata = indata['rf_data']
            
    # Split received payload into comma delineated chunks
    splitData = rfdata.split(',')

    # interpret the received payload
    interpretedData = interpretData(splitData)
    

def main():
    time.sleep(15)
    while True:

        # Web server sleeps 250 ms each call. This loop acts as a timer.
        i = 0
        while i < 40: # 40 iterations = 10 seconds
            checkWebServer()
            i += 1

        # Request valve state from controller
        sendToController("16,1") # hard coding zone 1 for now

        

if __name__ == '__main__':
    main()
