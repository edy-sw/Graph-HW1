import sys
import random
from Graph import Graph

def read_file(filename):
    try:
        with open(filename, 'r') as f:
            lines = f.read().split()
            if not lines:
                raise ValueError("Empty file")
            
            nr_v = int(lines[0])
            nr_e = int(lines[1])
            
            g = Graph(nr_v)
            
            idx = 2
            for _ in range(nr_e):
                v1 = int(lines[idx])
                v2 = int(lines[idx+1])
                cost = int(lines[idx+2])
                g.addEdge(v1, v2, cost)
                idx += 3
                
            return g
    except Exception as e:
        print(f"Error reading file: {e}")
        raise e

def generate_graph(nr_v, nr_e):
    g = Graph(nr_v)
    edges_added = 0
    while edges_added < nr_e:
        v1 = random.randint(0, nr_v - 1)
        v2 = random.randint(0, nr_v - 1)
        cost = random.randint(0, 299)
        if not g.isEdge(v1, v2):
            g.addEdge(v1, v2, cost)
            edges_added += 1
    return g

def start_read():
    print("Graph program: 1 - generate, 2 - read from file")
    try:
        cmd = int(input().strip())
        if cmd not in [1, 2]:
            print("Invalid")
            raise Exception("Invalid option")
            
        if cmd == 2:
            filename = input("Enter text name: ").strip()
            return read_file(filename)
        else:
            nr_v = int(input("Please enter number of vertices: ").strip())
            nr_e = int(input("Please enter number of edges: ").strip())
            return generate_graph(nr_v, nr_e)
    except Exception as e:
        raise e

def print_menu(g):
    menu = """Graph Operations
    1. Get the number of vertices
    2. The set of vertices
    3. Check if there is an edge between 2 vertices
    4. In degree of a vertex
    5. Out degree of a vertex
    6. The outbound edges of a vertex
    7. The inbound edges of a vertex
    8. Get the cost of an edge
    9. Modify the cost of an edge
    10. Add edge
    11. Remove edge
    12. Add vertex
    13. Remove vertex
    14. Save graph to file
    15. To String Graph
    16. Find Connected Components
    0. Exit"""
    print(menu)
    cmd = int(input("Command: ").strip())
    
    if cmd == 1:
        print(g.nrVertices())
    elif cmd == 2:
        vertices = g.setOfVertices()
        print(" ".join(map(str, vertices)))
    elif cmd == 3:
        v1, v2 = map(int, input("Vertices: ").split())
        print(1 if g.isEdge(v1, v2) else 0)
    elif cmd == 4:
        v1 = int(input("Vertex: ").strip())
        print(g.inDegree(v1))
    elif cmd == 5:
        v1 = int(input("Vertex: ").strip())
        print(g.outDegree(v1))
    elif cmd == 6:
        v1 = int(input("Vertex: ").strip())
        for e in g.outboundEdge(v1):
            print(e)
    elif cmd == 7:
        v1 = int(input("Vertex: ").strip())
        for e in g.inboundEdge(v1):
            print(e)
    elif cmd == 8:
        v1, v2 = map(int, input("Edge: ").split())
        print(g.getCost(v1, v2))
    elif cmd == 9:
        print("Edge: ", end="")
        v1, v2 = map(int, input().split())
        cost = int(input("Cost: "))
        g.changeCost(v1, v2, cost)
    elif cmd == 10:
        print("Edge: ", end="")
        v1, v2 = map(int, input().split())
        cost = int(input("Cost: "))
        g.addEdge(v1, v2, cost)
    elif cmd == 11:
        v1, v2 = map(int, input("Edge: ").split())
        g.removeEdge(v1, v2)
    elif cmd == 12:
        g.addVertex(g.nrVertices())
    elif cmd == 13:
        v1 = int(input("Vertex: ").strip())
        g.removeVertex(v1)
    elif cmd == 14:
        filename = input("File name: ").strip()
        print(f"The copy of the graph is saved to {filename}...")
        with open(filename, 'w') as f:
            f.write(g.toString())
    elif cmd == 15:
        print(g.toString(), end="")
    elif cmd == 16:
        components = g.connectedComponents()
        print(f"Number of connected components: {len(components)}")
        for i, comp in enumerate(components):
            print(f"Component {i + 1}: {' '.join(map(str, comp))}")
    elif cmd == 0:
        raise StopIteration()
    else:
        print("Invalid command")

def main():
    try:
        g = start_read()
    except Exception:
        print("Invalid data")
        return

    while True:
        try:
            print_menu(g)
        except StopIteration:
            print("Exiting")
            break
        except Exception as e:
            print(f"Operation failed or invalid input: {e}")

if __name__ == "__main__":
    sys.setrecursionlimit(100000)
    main()
