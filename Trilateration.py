def trilaterate(x1, y1, x2, y2, x3, y3, r1, r2, r3):
    a1 = (2*x2) - (2*x1)
    b1 = (2*y2) - (2*y1)
    c1 = r1**2 - r2**2 - x1**2  - y1**2 + x2**2 + y2**2

    a2 = (2*x3) - (2*x1)
    b2 = (2*y3) - (2*y1)
    c2 = r1**2 - r3**2 - x1**2 - y1**2 + x3**2 + y3**2

    y = (c2*a1 - a2*c1)/(a1*b2 - a2*b1)
    x = (c1 - b1*y)/a1

    return x, y

circle1 = [0,0]
circle2 = [2,0]
circle3 = [0,2]
distances = [2,2,2]

print(trilaterate(circle1[0], circle1[1], circle2[0], circle2[1], circle3[0], circle3[1], distances[0], distances[1], distances[2]))
