# libraries
import re, ast, sys, math
# import string
from math import sqrt

# Global Variables
startflag = 0
streets = []

intersectionTupples = []
intersectionPoints = []
vIndex = 1


# Classes

class Point:
    # constructor for a point
    def __init__(self, x=0.0, y=0.0, street="", intersection=False):
        self.x = float(x)
        self.y = float(y)
        self.intersection = intersection
        self.street = street


class Line:
    # constructor for a line (it takes Points as arguments)
    def __init__(self, p1, p2):
        self.A = Point(p1.x, p1.y)  # example of instanciating
        self.B = Point(p2.x, p2.y)


class Street:
    # constructor for a street
    def __init__(self, line_list, name=""):
        self.lines = line_list
        self.name = name


class Route:
    # constructor for a Route
    def __init__(self, street_list):
        self.streets = street_list


class IntersectionTupple:
    def __init__(self, p1, p2, p3, intersection=False):
        self.A = Vertex(p1.x, p1.y)
        self.B = Vertex(p2.x, p2.y)
        self.C = Vertex(p3.x, p3.y)
        self.intersection = intersection


class Vertex:
    # constructor for a Vertex
    def __init__(self, x=0.0, y=0.0, index=0, intersection=False):
        self.x = float(x)
        self.y = float(y)
        self.index = index
        self.intersection = intersection


class Edge:
    # constructor for an Edge
    def __init__(self, p1, p2):
        self.A = Vertex(p1.x, p1.y)
        self.B = Vertex(p2.x, p2.y)


# Methods

def arePointsCollinear(p0, intersection, p2):  # ref from StackOverflow
    x1, y1 = intersection[0] - p0[0], intersection[1] - p0[1]
    x2, y2 = p2[0] - p0[0], p2[1] - p0[1]
    if abs(x1 * y2 - x2 * y1) < 1e-12:
        return True
    else:
        return False


def findEdges(vertices, intersectionTupples, intersectionPoints):
    tempEdges = []
    for Tupple in intersectionTupples:
        p0 = (Tupple.A.x, Tupple.A.y)
        p1 = (Tupple.B.x, Tupple.B.y)
        p2 = (Tupple.C.x, Tupple.C.y)
        point1 = Point(Tupple.A.x, Tupple.A.y)
        point2 = Point(Tupple.B.x, Tupple.B.y)
        point3 = Point(Tupple.C.x, Tupple.C.y)
        multiplePointsList = []
        newList = []
        for iPoint in intersectionPoints:
            iCoord = (iPoint.x, iPoint.y)
            if arePointsCollinear(iCoord, p0, p2) and iCoord != p1:
                multiplePointsList.append(iCoord)

        if multiplePointsList == []:
            multiplePointsList.append(p0)
            multiplePointsList.append(p2)
            multiplePointsList.append(p1)
            tempEdges.append((p0, p1))
            tempEdges.append((p1, p2))

        else:
            multiplePointsList.append(p0)
            multiplePointsList.append(p2)
            multiplePointsList.append(p1)
            multiplePointsList = list(set(multiplePointsList))

            if p2[0] - p0[0] == 0 or p2[1] - p0[1] == 0:
                newList = sorted(multiplePointsList, key=lambda x: (float(x[0])), reverse=False)
            else:
                newList = sorted(multiplePointsList, key=lambda x: (float(x[0])), reverse=False)
        for k in range(0, len(newList) - 1):
            tempEdges.append((newList[k], newList[k + 1]))
    getEdgeIndex(list(set(tempEdges)), vertices)


def getEdgeIndex(edgeList, vertices):
    sys.stdout.write("\nE {")
    sys.stdout.flush()
    for i in range(len(edgeList)):
        index1 = 0
        index2 = 0
        for j in range(len(vertices)):
            if edgeList[i][0][0] == vertices[j].x and edgeList[i][0][1] == vertices[j].y:
                index1 = vertices[j].index
            if edgeList[i][1][0] == vertices[j].x and edgeList[i][1][1] == vertices[j].y:
                index2 = vertices[j].index
        if index1 != index2:
            if i == len(edgeList) - 1:
                sys.stdout.write("<" + str(index1-1) + "," + str(index2-1) + ">")
                sys.stdout.flush()
            else:
                sys.stdout.write("<" + str(index1-1) + "," + str(index2-1) + ">,")
                sys.stdout.flush()
        edges.append(Edge(Vertex(edgeList[i][0][0], edgeList[i][0][1], int(index1), True),
                          Vertex(edgeList[i][1][0], edgeList[i][1][1], int(index2), True)))
    sys.stdout.write("}\n")
    sys.stdout.flush()


def printEdges():
    sys.stdout.write("E = {")
    sys.stdout.flush()
    for i, edge in enumerate(edges):
        sys.stdout.write("  <" + str(int(edge.A.index)-1) + "," + str(int(edge.B.index)-1) + ">,")
        sys.stdout.flush()
    sys.stdout.write("}")
    sys.stdout.flush()


def displayError(error):
    print("Error: {}".format(error))


def nearCheck(x, y, tolerence1=1e-09, tolerence2=0.0):
    return abs(x - y) <= max(tolerence1 * max(abs(x), abs(y)), tolerence2)


def betweenLineCheck(a, b, c):
    distance1 = sqrt((a.x - c.x) ** 2 + (a.y - c.y) ** 2)
    distance2 = sqrt((c.x - b.x) ** 2 + (c.y - b.y) ** 2)
    distance3 = sqrt((a.x - b.x) ** 2 + (a.y - b.y) ** 2)
    return nearCheck(distance1 + distance2, distance3)


def lineIntersectionCheck(line1, line2):
    try:
        x1, y1 = line1.A.x, line1.A.y
        x2, y2 = line1.B.x, line1.B.y
        x3, y3 = line2.A.x, line2.A.y
        x4, y4 = line2.B.x, line2.B.y

        numm_x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
        deno_x = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
        xintersect = numm_x / deno_x

        numm_y = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
        deno_y = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
        yintersect = numm_y / deno_y

        intersectionn = Point(xintersect, yintersect)
        if betweenLineCheck(line1.A, line1.B, intersectionn) and betweenLineCheck(line2.A, line2.B, intersectionn):
            return xintersect, yintersect
        else:
            return None, None
    except Exception as exp:  # devide by zero
        return None, None


def addStreet(command, streetName, coordinateArray):
    for street in streets:
        if streetName == street.name:
            displayError("Street Already exists")
            return

    for point in coordinateArray:
        fetchedPoints.append(Point(point[0], point[1], streetName))

    l = len(fetchedPoints)
    for index, point in enumerate(fetchedPoints):
        if index < (l - 1):
            lines.append(Line(fetchedPoints[index], fetchedPoints[index + 1]))

    streets.append(Street(lines, streetName))


def validation(command, streetName, coordinateArray):
    if command == "a":
        addStreet(command, streetName, coordinateArray)
    elif command == "c":
        removeStreet(streetName)
        addStreet(command, streetName, coordinateArray)


def generateGraph():
    findIntersections()


def printVertex(vertices):
    sys.stdout.write("V " + str(len(vertices)))
    # sys.stdout.flush()
    # for i, vertex in enumerate(vertices):
    #     sys.stdout.write("  " + str(vertex.index) + ":  ({0:.2f},".format(vertex.x) + "{0:.2f})".format(vertex.y))
    #     sys.stdout.flush()
    # sys.stdout.write("}")
    sys.stdout.flush()


def findIntersections():
    global vIndex
    vertices = []
    vIndex = 1
    intersectionTupples = []
    intersectionPoints = []

    for i1 in range(0, len(streets)):
        for i2 in range(i1 + 1, len(streets)):
            for line1 in streets[i1].lines:
                for line2 in streets[i2].lines:

                    x, y = lineIntersectionCheck(line1, line2)
                    xin, yin = lineIntersectionCheck(line1, line2)
                    if x is not None:
                        flag = 0
                        intersectionPoints.append(Vertex(xin, yin))
                        for index, vertex in enumerate(vertices):
                            if vertices[index].x == x and vertices[index].y == y:
                                flag = 1
                        if flag == 0:
                            vertices.append(Vertex(x, y, vIndex, True))
                            vIndex += 1
                        else:
                            flag = 0

                        x, y = line1.A.x, line1.A.y
                        for index, vertex in enumerate(vertices):
                            if vertices[index].x == x and vertices[index].y == y:
                                flag = 1
                        if flag == 0:
                            vertices.append(Vertex(x, y, vIndex, False))
                            vIndex += 1
                        else:
                            flag = 0

                        x, y = line1.B.x, line1.B.y
                        for index, vertex in enumerate(vertices):
                            if vertices[index].x == x and vertices[index].y == y:
                                flag = 1
                        if flag == 0:
                            vertices.append(Vertex(x, y, vIndex, False))
                            vIndex += 1
                        else:
                            flag = 0

                        x, y = line2.A.x, line2.A.y
                        for index, vertex in enumerate(vertices):
                            if vertices[index].x == x and vertices[index].y == y:
                                flag = 1
                        if flag == 0:
                            vertices.append(Vertex(x, y, vIndex, False))
                            vIndex += 1
                        else:
                            flag = 0

                        x, y = line2.B.x, line2.B.y
                        for index, vertex in enumerate(vertices):
                            if vertices[index].x == x and vertices[index].y == y:
                                flag = 1
                        if flag == 0:
                            vertices.append(Vertex(x, y, vIndex, False))
                            vIndex += 1
                        else:
                            flag = 0
                        x1, y1 = line1.A.x, line1.A.y
                        x2, y2 = line1.B.x, line1.B.y
                        x3, y3 = line2.A.x, line2.A.y
                        x4, y4 = line2.B.x, line2.B.y

                        intersectionTupples.append(IntersectionTupple(Vertex(x1, y1), Vertex(xin, yin), Vertex(x2, y2)))
                        intersectionTupples.append(IntersectionTupple(Vertex(x3, y3), Vertex(xin, yin), Vertex(x4, y4)))

    new_set = set()
    for i in range(len(vertices)):
        new_set.add((vertices[i].x, vertices[i].y, vertices[i].index))
    printVertex(vertices)
    findEdges(vertices, intersectionTupples, intersectionPoints)


def removeStreet(streetName):
    # print(streets)
    stFlag = 1
    if streets == []:
        displayError("No street exists")
    for stIndex in range(len(streets)):
        # print(streets[stIndex].name,streetName,stIndex,streets[stIndex])
        if streets[stIndex].name.lower() == streetName.lower():
            # print(streets)
            del streets[stIndex]
            # print(streets)
            stFlag = 0
            break
    if stFlag == 1:
        displayError("street does not exist")


def parser(input):
    command = ""
    streetName = ""
    coordinateArray = ""

    commandParser = re.compile("^(a|r|c|g)\s*")
    command = commandParser.findall(input)
    if command == []:
        displayError("Invalid input command/format")
        return
    if command[0] == "g":
        generateGraph()
    elif command[0] == "a":
        parserRegex = re.compile("(a|r|c|g)\s+(\"[a-zA-Z ]+?\")\s+((\([-]?[0-9]+,[-]?[0-9]+\)\s*)*)$")
        inputList = parserRegex.findall(input)
        if inputList == []:
            displayError("command arguments incomplete")
            return
        if len(inputList[0]) > 2 and len(inputList) > 0:
            command = inputList[0][0]
            streetName = inputList[0][1].replace("\"", "")
            coordinateString = inputList[0][2]

            coordinateRegex = re.compile(r'\s*\([-]?[0-9]+,[-]?[0-9]+\)\s*')
            coordinates = coordinateRegex.findall(coordinateString)
            coordinateArray = [eval(x) for x in coordinates]

            validation(command, streetName, coordinateArray)
        else:
            displayError("Invalid input command/format")
    elif command[0] == "c":
        parserRegex = re.compile("(a|r|c|g)\s+(\"[a-zA-Z ]+?\")\s+((\([-]?[0-9]+,[-]?[0-9]+\)\s*)*)$")
        inputList = parserRegex.findall(input)
        if inputList == []:
            displayError("command arguments incomplete")
            return
        if len(inputList[0]) > 2 and len(inputList) > 0:
            command = inputList[0][0]
            streetName = inputList[0][1].replace("\"", "")
            coordinateString = inputList[0][2]

            coordinateRegex = re.compile(r'\s*\([-]?[0-9]+,[-]?[0-9]+\)\s*')
            coordinates = coordinateRegex.findall(coordinateString)

            coordinateArray = [eval(x) for x in coordinates]

            validation(command, streetName, coordinateArray)
    elif command[0] == "r":
        parserRegex = re.compile("^(a|r|c|g)\s+(\"[a-zA-Z ]+?\")\s*$")
        inputList = parserRegex.findall(input)
        if inputList == []:
            displayError("Invalid input command")
            return
        command = inputList[0][0]
        streetName = inputList[0][1].replace("\"", "")
        removeStreet(streetName)

    elif len(command[0]) > 0:
        print("Error: \"{}\" is not a valid command", format(command))
    return


# main function
def main():
    ### YOUR MAIN CODE GOES HERE

    ### sample code to read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment

    global fetchedPoints
    global lines
    global vertices
    global edges
    fetchedPoints = []
    lines = []
    vertices = []
    edges = []
    try:
        while True:
            line = sys.stdin.readline()
            if line == '':
                break
            parser(line)
            fetchedPoints = []
            lines = []
        # return exit code 0 on successful termination
        # sys.exit(0)
    except KeyboardInterrupt:
        sys.exit(0)
    except(BrokenPipeError, IOError):
        # sys.stdout.write("THIS IS A BROKEN PIPE")
        # sys.stdout.flush()
        sys.exit(0)
        pass
    sys.exit(0)


if __name__ == '__main__':
    main()

