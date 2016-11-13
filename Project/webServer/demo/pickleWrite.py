import pickle

shared = {"Foo":"Bar", "Honey":"Badger", "Shit":"Storm"}
shared["Foo"] = "Updated Bar Value"
shared["NewKey"] = "New Key"
shared["DynamicTest"] = "Test"
fp = open("shared.pkl", "w")
pickle.dump(shared, fp)
