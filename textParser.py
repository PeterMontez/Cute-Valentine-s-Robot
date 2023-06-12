import random

finalLines = []
fixed = []

with open('sentences.txt') as f:
    lines = f.readlines()

for i in range (len(lines)):
    lines[i] = lines[i].strip()
    count = 0;
    words = lines[i].split(' ')
    finalWord = ''
    for j in range (len(words)):
        count = count + len(words[j])
        count += 1
        if (count > 17 or words[j] == '/'):
            finalWord = finalWord[:-1]
            finalWord += ','
            count = len(words[j]) + 1
        
        if(words[j] != '/'):
            finalWord += words[j]
            
        if (j != len(words) and words[j] != '/'):
            finalWord += ' '
            
    
    if (finalWord.count(',') > 1):
        print(f"ERROR : {finalWord}")
    else:
        finalWord = finalWord[:-1]
        fixed.append(finalWord)
        
finalLines = fixed.copy()   

while (len(finalLines) < 50):
    finalLines.append(fixed[random.randint(0, len(fixed)-1)])
    
print('\n')

for i in range (len(finalLines)):
    print(finalLines[i], end='')
    if (i != len(finalLines)):
        print(';', end='')