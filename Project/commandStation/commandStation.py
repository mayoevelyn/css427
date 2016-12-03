import serial
import pickle
from xbee import ZigBee
from datetime import datetime
from collections import defaultdict

"""serial_port = serial.Serial('/dev/ttyUSB0', 9600)
xbee = ZigBee(serial_port)
"""

sensorAverageCount = 0
lightAverage = 0
temperatureAverage = 0
humidityAverage = 0
moistureAverage = 0


def sendToController(outdata):
    """global xbee
    DEST_ADDR_LONG = "\x00\x13\xA2\x00\x40\xE3\xCD\x1E"
    DEST_ADDR = "\xFF\xFE"

    # Send packet
    xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data=outdata)            
        
    # Wait for ack
    ack = xbee.wait_read_frame()
    """
    print "test send to controller"

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
    if moisture > threshold:
        print moisture + threshold
        print "moisture: " + str(moisture)
        print "threshold: " + str(threshold)
        return True
    else:
        return False

def setSchedule(zone, lightAverage, temperatureAverage, humidityAverage, moistureAverage):
    # determine if the schedule must be set and set it if needed, for now ignore zone
    #print "trace setSchedule"
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
        print "test not humid and not moist"
        return

    if bright and not moist:
        # water: sunny and not moist
        schedule = "25," + str(zone) + "," + str(scheduleHour) + "," + str(scheduleMinute) + "," + "1"
        sendToController(schedule)
        print "test sunny and not moist"
        return

    if hot and not moist:
        # water: hot and not moist
        schedule = "25," + str(zone) + "," + str(scheduleHour) + "," + str(scheduleMinute) + "," + "1"
        print "test hot and not moist"
        sendToController(schedule)
        return
    
    if hot and not humid and bright:
        # water: hot and not humid and sunny
        schedule = "25," + str(zone) + "," + str(scheduleHour) + "," + str(scheduleMinute) + "," + "1"
        sendToController(schedule)
        print "test hot and not humid and sunny"
        return

    print("don't water:\nhot: " + str(hot) + "\nhumid: " + str(humid) + "\nbright: " + str(bright) + "\nmoist: " + str(moist))
    
    

def updateAverages(zone, light, temperature, humidity, moisture):
    # update sensor averages, for now ignore zone
    #print "trace updateAverages"
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

        print "sensorAverageCount " + str(sensorAverageCount)
        print "lightAverage " + str(lightAverage)
        print "temperatureAverage " + str(temperatureAverage)
        print "humidityAverage " + str(humidityAverage)
        print "moistureAverage " + str(moistureAverage)

        currentAverages = {
            "Temperature":str(temperatureAverage) + "C",
            "Humidity":str(humidityAverage) + "%",
            "Brightness":str(lightAverage) + "L",
            "Moisture":str(moistureAverage) + "%"}

        fp = open("../webServer/sprinklerPage/cmdStationData/averages.pkl", "w")
        pickle.dump(currentAverages, fp)
        fp.close()
        print str(moistureAverage) + " before setSchedule"
        setSchedule(zone, lightAverage, temperatureAverage, humidityAverage, moistureAverage)
    

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
    print "stub"
    
            

def main():       
    """global xbee    

    while True:
        try:
            # Receive data
            indata = xbee.wait_read_frame()

            # Extract payload
            rfdata = indata['rf_data']
            
            # Split received payload into comma delineated chunks
            splitData = rfdata.split(',')

            # interpret the received payload
            interpretedData = interpretData(splitData)            
            
        except KeyboardInterrupt:
            break

    serial_port.close()
    """
    print "testing"
    testdata = ["50", "1", "99", "0", "99", "0"]    
    interpretData(testdata)
    interpretData(testdata)
    interpretData(testdata)
    interpretData(testdata)
    

if __name__ == '__main__':
    main()
