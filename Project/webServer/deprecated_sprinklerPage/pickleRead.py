import pickle

fp = open("fromForm.pkl")
data = pickle.load(fp)

print data["Name"]

