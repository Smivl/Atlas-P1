import json
import os
import pickle
import random
from re import M
import nltk
import tensorflow as tf
from nltk.stem import WordNetLemmatizer
import numpy as np
from tensorflow.keras.models import load_model

pathList = [
    'Main',
    'Audio'
]

returnType = [
    'tag',
    'channel'
]

modelsList = []
wordsList = []
classesList = []

for path in pathList:
    modelD = load_model(f'src\system\Training\Models\{path}\model.h5')
    modelsList.append(modelD)

    wordsD = pickle.load(open(f'src\system\Training\Models\{path}\words.pkl', 'rb'))
    wordsList.append(wordsD)

    classesD = pickle.load(open(f'src\system\Training\Models\{path}\classes.pkl', 'rb'))
    classesList.append(classesD)

lemmatizer = WordNetLemmatizer()

def clean_up_sentence(sentence):

    sentence_words = nltk.word_tokenize(sentence)
    sentence_words = [lemmatizer.lemmatize(word) for word in sentence_words]
    return sentence_words

def bag_of_words(sentence, index):
    words = wordsList[index]

    sentence_words = clean_up_sentence(sentence)
    bag = [0] * len(words)
    for w in sentence_words:
        for i, word in enumerate(words):
            if word == w:
                bag[i] = 1
    return np.array(bag)

def predict_class(sentence, index):
    model = modelsList[index]
    classes = classesList[index]

    bow = bag_of_words(sentence, index)
    res = model.predict(np.array([bow]), verbose = 0)[0]
    ERROR_THRESHOLD = 0.25
    results = [[i, r] for i, r in enumerate(res) if r > ERROR_THRESHOLD]

    results.sort(key=lambda x: x[1], reverse=True)
    return_list = []
    for r in results:
        return_list.append({f'{returnType[index]}': classes[r[0]], 'probability': str(r[1])})

    return return_list

def get_result(sentence, index = 0):
    sentence.lower()
    result_list = predict_class(sentence, index)
    result = result_list[0][returnType[index]]
    return result


# MAIN LAYER FUNCTIONS

def get_all_responses(sentence):
    sentence.lower()
    intents_json = json.loads(open('src\system\MainLayer.json').read())
    intents_list = predict_class(sentence, 0)

    tag = intents_list[0]['intent']
    list_of_intents = intents_json['intents']

    for i in list_of_intents:
        if i['tag'] == tag:
            return i['responses']
         
def get_random_response(sentence):
    sentence.lower()
    intents_json = json.loads(open(f'src\system\Mainlayer.json').read())
    intents_list = predict_class(sentence, 0)

    tag = intents_list[0]['intent']
    list_of_intents = intents_json['intents']
    for i in list_of_intents:
        if i['tag'] == tag:
            result = random.choice(i['responses'])
            break

    return result

def get_responses_from_tag(tag):

    result = "NULL"

    intents_json = json.loads(open('src\system\intents.json').read())
    list_of_intents = intents_json['intents']

    for i in list_of_intents:
        if i['tag'] == tag:
            result = random.choice(i['responses'])
            break

    return result