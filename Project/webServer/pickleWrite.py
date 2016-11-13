import pickle

shared = {"Foo":"Bar", "Honey":"Badger"}
fp = open("shared.pkl", "w")
pickle.dump(shared, fp)
