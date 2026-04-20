import random
import copy

class Graph:
    def __init__(self, nr_vertices=0):
        self.inMap = {}
        self.outMap = {}
        self.costMap = {}
        for i in range(nr_vertices):
            self.inMap[i] = []
            self.outMap[i] = []

    def copy(self):
        new_graph = Graph()
        new_graph.inMap = copy.deepcopy(self.inMap)
        new_graph.outMap = copy.deepcopy(self.outMap)
        new_graph.costMap = copy.deepcopy(self.costMap)
        return new_graph

    def addEdge(self, v1, v2, cost):
        if not self.isVertex(v1) or not self.isVertex(v2) or self.isEdge(v1, v2):
            raise Exception("Invalid edge addition.")
        self.costMap[(v1, v2)] = cost
        self.outMap[v1].append(v2)
        self.inMap[v2].append(v1)

    def isVertex(self, v):
        return v in self.inMap

    def isEdge(self, v1, v2):
        return v2 in self.outMap.get(v1, [])

    def setOfVertices(self):
        return list(self.outMap.keys())

    def nrVertices(self):
        return len(self.outMap)

    def nrEdges(self):
        return len(self.costMap)

    def inDegree(self, x):
        return len(self.inMap[x])

    def outDegree(self, x):
        return len(self.outMap[x])

    def getCost(self, x, y):
        if not self.isEdge(x, y):
            raise Exception("Edge does not exist.")
        return self.costMap[(x, y)]

    def changeCost(self, x, y, val):
        if not self.isEdge(x, y):
            raise Exception("Edge does not exist.")
        self.costMap[(x, y)] = val

    def inboundEdge(self, x):
        return self.inMap[x]

    def outboundEdge(self, x):
        return self.outMap[x]

    def removeEdge(self, x, y):
        if not self.isEdge(x, y):
            raise Exception("Edge does not exist.")
        
        # Remove from cost map
        if (x, y) in self.costMap:
            del self.costMap[(x, y)]
            
        # Remove from inbound and outbound maps
        if x in self.inMap[y]:
            self.inMap[y].remove(x)
        if y in self.outMap[x]:
            self.outMap[x].remove(y)

    def addVertex(self, val):
        if self.isVertex(val):
            raise Exception("Vertex already exists.")
        self.inMap[val] = []
        self.outMap[val] = []

    def removeVertex(self, val):
        if not self.isVertex(val):
            return

        del self.inMap[val]
        del self.outMap[val]

        # Re-index cost map
        newCostMap = {}
        for (x, y), cost in list(self.costMap.items()):
            if x == val or y == val:
                continue
            new_x = x - 1 if x > val else x
            new_y = y - 1 if y > val else y
            newCostMap[(new_x, new_y)] = cost
        self.costMap = newCostMap

        # Re-index inMap
        newInMap = {}
        for k, v_list in self.inMap.items():
            if val in v_list:
                v_list.remove(val)
            
            new_k = k - 1 if k > val else k
            newInMap[new_k] = [(item - 1 if item > val else item) for item in v_list]
        self.inMap = newInMap

        # Re-index outMap
        newOutMap = {}
        for k, v_list in self.outMap.items():
            if val in v_list:
                v_list.remove(val)
            
            new_k = k - 1 if k > val else k
            newOutMap[new_k] = [(item - 1 if item > val else item) for item in v_list]
        self.outMap = newOutMap

    def toString(self):
        lines = []
        lines.append(f"{self.nrVertices()} {self.nrEdges()}")
        for (u, v), cost in self.costMap.items():
            lines.append(f"{u} {v} {cost}")
        return "\n".join(lines) + "\n"

    def _dfs(self, v, visited, component):
        visited[v] = True
        component.append(v)
        
        for neighbor in self.outMap[v]:
            if not visited[neighbor]:
                self._dfs(neighbor, visited, component)
                
        for neighbor in self.inMap[v]:
            if not visited[neighbor]:
                self._dfs(neighbor, visited, component)

    def connectedComponents(self):
        visited = {v: False for v in self.outMap.keys()}
        components = []
        
        for v in self.outMap.keys():
            if not visited[v]:
                component = []
                self._dfs(v, visited, component)
                components.append(component)
                
        return components
