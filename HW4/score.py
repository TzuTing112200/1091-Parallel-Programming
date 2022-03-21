Y = 0
F = 0
T = 0

while True:
    
    F = eval(input('F = '))
    T = F * 1.5

    while True:
        Y = eval(input('Y = '))
        if Y == -99:
            break
        
        S = (T - Y) / (T - F) * 60

        if Y < F * 1.5:
            S += 20
        if Y < F * 2:
            S += 20

        print('S =', S)
        
