import pickle
import time
from threading import Thread

valveSeqNum = 1
sensorsSeqNum = 1
preferencesSeqNum = 1

while True:
    time.sleep(0.25)
    
    ############################################
    # Valve toggle
    
    try:
        fp = open("formData/formValveState.pkl")
        formValveState = pickle.load(fp)
    
        if valveSeqNum <= formValveState["SeqNum"]:            
            # do work here
            print("\n" + str(valveSeqNum) + " Valve State: " + formValveState["ValveState"])

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
        fp = open("formData/formSensors.pkl")
        formSensors = pickle.load(fp)
    
        if sensorsSeqNum <= formSensors["SeqNum"]:
            # do work here
            print("\n" + str(sensorsSeqNum) + " Sensor Thresholds")
            
            try:
                tThresh = int(formSensors["TemperatureThreshold"])
                print("Temperature Threshold: " + str(tThresh))
            except:
                pass

            try:
                hThresh = int(formSensors["HumidityThreshold"])
                print("Humidity Threshold: " + str(hThresh))
            except:
                pass

            try:
                bThresh = int(formSensors["BrightnessThreshold"])
                print("Brightness Threshold: " + str(bThresh))
            except:
                pass

            try:
                mThresh = int(formSensors["MoistureThreshold"])
                print("Moisture Threshold: " + str(mThresh))
            except:
                pass

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
        fp = open("formData/formPreferences.pkl")
        formPreferences = pickle.load(fp)
    
        if preferencesSeqNum <= formPreferences["SeqNum"]:
            # do work here
            print("\n" + str(preferencesSeqNum) + " Preferences")

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
        
    
    

    

