from time import time
from tkinter import messagebox as mb


def print_info(string_info):
    mb.showinfo(title="Информация", message=string_info)

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

def find_intersections(edge):
    lst = []
    if edge[0].y == edge[1].y:
        return []

    if edge[0].y > edge[1].y:
        edge[0], edge[1] = edge[1], edge[0]

    x_start = edge[0].x
    y_start = edge[0].y
    x_end = edge[1].x
    y_end = edge[1].y

    dy = 1
    dx = (x_end - x_start) / (y_end - y_start)
    x = x_start
    y = y_start

    while y < y_end:
        lst.append([int(x), int(y)])
        y += dy
        x += dx
    return lst

def fill_edges(my_canvas, edges):
    for edge in edges:
        my_canvas.print_edges(edge[0].x, edge[0].y, edge[1].x, edge[1].y)

def make_edges_list(figures):
    edges = list()
    for fig in figures:
        amount_point = len(fig)
        for i in range(amount_point):
            if i + 1 > amount_point - 1:
                edges.append([fig[-1], fig[0]])
            else:
                edges.append([fig[i], fig[i + 1]])

    return edges


def fill_line(my_canvas,xstart, xend, y):
    for x in range(xstart, xend , 1):
        my_canvas.reverse_pixel(x,y)

#Каждая фигура отдельно
def fill_figure(my_canvas, figures):
    time_start = time()
    for fig in figures:
        edges = list()
        amount_point = len(fig)
        for i in range(amount_point):
            if i + 1 > amount_point - 1:
                edges.append([fig[-1], fig[0]])
            else:
                edges.append([fig[i], fig[i + 1]])
        max_x = edges[0][0].x
        for i in range(len(edges)):
            for j in range(len(edges[i])):
                if (edges[i][j].x > max_x):
                    max_x = edges[i][j].x
        for edge in edges:
            intersections = find_intersections(edge)
            for i in range(len(intersections)):
                fill_line(my_canvas, intersections[i][0] + 1, max_x + 1, intersections[i][1])
    time_end = time()
    str_res = str(round(time_end - time_start, 3)) + " секунд"
    print_info("Время выполнения закраски = " + str_res)

#Каждая фигура отдельно
def fill_figure_with_delay(my_canvas, figures):
    for fig in figures:
        edges = list()
        amount_point = len(fig)
        for i in range(amount_point):
            if i + 1 > amount_point - 1:
                edges.append([fig[-1], fig[0]])
            else:
                edges.append([fig[i], fig[i + 1]])
        max_x = edges[0][0].x
        for i in range(len(edges)):
            for j in range(len(edges[i])):
                if (edges[i][j].x > max_x):
                    max_x = edges[i][j].x
        for edge in edges:
            intersections = find_intersections(edge)
            for i in range(len(intersections)):
                my_canvas.canvas.after(1, fill_line(my_canvas, intersections[i][0] + 1, max_x + 1, intersections[i][1]))
                my_canvas.canvas.update()









# def fill_figure(my_canvas, figures):
#     edges = make_edges_list(figures)
#     time_start = time()
#     max_x = edges[0][0].x
#     for i in range(len(edges)):
#         for j in range(len(edges[i])):
#             if (edges[i][j].x > max_x):
#                 max_x = edges[i][j].x
#     for edge in edges:
#         intersections = find_intersections(edge)
#         for i in range(len(intersections)):
#             fill_line(my_canvas, intersections[i][0] + 1, max_x + 1, intersections[i][1])
    
#     fill_edges(my_canvas, edges)
#     time_end = time()
#     str_res = str(round(time_end - time_start, 3)) + " секунд"
#     print_info("Время выполнения закраски = " + str_res)



# def fill_figure_with_delay(my_canvas, figures):
#     edges = make_edges_list(figures)
#     max_x = edges[0][0].x
#     for i in range(len(edges)):
#         for j in range(len(edges[i])):
#             if (edges[i][j].x > max_x):
#                 max_x = edges[i][j].x
#     for edge in edges:
#         intersections = find_intersections(edge)
#         for i in range(len(intersections)):
#             my_canvas.canvas.after(1, fill_line(my_canvas, intersections[i][0] + 1, max_x + 1, intersections[i][1]))
#             my_canvas.canvas.update()
#     fill_edges(my_canvas, edges)