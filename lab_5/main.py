from tkinter import messagebox
from tkinter import ttk
from tkinter.ttk import *
import tkinter as tk
from tkinter import font
import sys
import math as m
import time
from dataclasses import dataclass

from algo import *
from settings import *
from mydraw import *

def show_task():
    messagebox.showinfo("")

def show_var():
    messagebox.showinfo("Вариант лабораторной работы", "Алгоритм заполнения по ребрам")
    
def show_info():
    messagebox.showinfo("Правила работы с программой", "-Значения координат должны быть целыми числами")
    
def error_input(message):
    messagebox.showerror("Ошибка","Ошибка ввода!\n" + message)

# figures - массив всех замкнутых фигур
# def make_edges_list(figures):
#     edges = list()
#     for fig in figures:
#         amount_point = len(fig)
#         for i in range(amount_point):
#             if i + 1 > amount_point - 1:
#                 edges.append([fig[-1], fig[0]])
#             else:
#                 edges.append([fig[i], fig[i + 1]])

#     return edges

''' -----------------------ВЫБОР ЦВЕТА И МЕТОДА ------------------------ '''
def fill_all_figures():
    if not allFigures and not currentFigure:
        messagebox.showwarning("Ошибка!", "Фигура не введена для закраски!")
    elif not allFigures and  currentFigure:
        messagebox.showwarning("Ошибка!", "Фигура не замкнута для закраски!")
    else:
        my_canvas.clear_all()
        edges = make_edges_list(allFigures)
        for edge in edges:
            my_canvas.print_edges(edge[0].x, edge[0].y, edge[1].x, edge[1].y)
        if not delay:
            fill_figure(my_canvas, allFigures)
        else:
            fill_figure_with_delay(my_canvas, allFigures)
        


def findIndexForListPointScroll(allArraysFigure, currentArray):
    index = 0

    for pointFigure in allArraysFigure:
         index += len(pointFigure) + 1

    index += len(currentArray)
    return index

def check_point_existence(x,y, arr):
    for point in arr:
        if (point.x == x and point.y == y):
            return True
    return False

def add_point(x, y):
    if (check_point_existence(x, y, currentFigure) == False):
        if len(currentFigure) != 0:
            my_canvas.print_line(currentFigure[-1].x, currentFigure[-1].y, x, y)

        index = findIndexForListPointScroll(allFigures, currentFigure)
        listPoint_scroll.insert(index,  "{:3d}) x = {:4d}; y = {:4d}".format(index + 1, x, y))
        currentFigure.append(Point(x, y))
    else:
        messagebox.showwarning("Ошибка!", "Точка с такими координатами фигуры уже введена!")

def add_point_figure_onClick(event):
    x, y = event.x,  event.y
    add_point(x, y)

def add_point_button():
    x = ent_x.get()
    y = ent_y.get()
    if not x or not y:
        messagebox.showinfo("Ошибка!", "Координаты точек не введены!")
    else:
        try:
            x = int(x)
            y = int(y)
            if (x < 0 or x > CANVAS_WIDTH or y < 0 or y > CANVAS_WIDTH):
                messagebox.showinfo("Ошибка!", "Координаты точки выходят за границы холста!")
                return
        except ValueError:
            messagebox.showinfo("Ошибка!", "Координаты точек должны быть целыми числами!")
            return
        add_point(x, y)


def close_figure():
    global currentFigure
    if len(currentFigure) > 2:
        my_canvas.print_line(currentFigure[-1].x, currentFigure[-1].y, currentFigure[0].x, currentFigure[0].y)

        index = findIndexForListPointScroll(allFigures, currentFigure)
        listPoint_scroll.insert(index, "-------Фигура добавлена-------")

        allFigures.append(currentFigure)
        currentFigure = []
    elif len(currentFigure) == 0:
        messagebox.showwarning("Ошибка!", "Точки фигуры не введены!")
    else:
        messagebox.showwarning("Ошибка!", "Фигуру нельзя замкнуть!\nЧтобы замкнуть фигуру, необходимо ввести как минимум 3 точки!")
def get_len():
    leng = 0 
    for fig in allFigures:
        leng += len(fig)
    leng += len(currentFigure)
    return leng

def delete_point():
    # мы можем получить удаляемый элемент по индексу
    # selected_language = languages_listbox.get(selection[0])
    ind = findIndexForListPointScroll(allFigures, currentFigure)
    listPoint_scroll.delete(ind-1)
    my_canvas.clear_all()

    if (len(currentFigure) != 0):
        currentFigure.pop()
    elif len(allFigures) > 0 and len(allFigures[-1]) > 0:
        allFigures[-1].pop()
    for g in range (len(allFigures)):
        fig = allFigures[g]
        if (g == len(allFigures) - 1):
            for i in range(len(fig) - 1):
                my_canvas.print_line(fig[i].x,fig[i].y, fig[i+1].x, fig[i+1].y)
        else:
            for i in range(len(fig)):
                if i + 1 > len(fig)- 1:
                    my_canvas.print_line(fig[-1].x,fig[-1].y, fig[0].x, fig[0].y)
                else:
                    my_canvas.print_line(fig[i].x,fig[i].y, fig[i+1].x, fig[i+1].y)
    for i in range(len(currentFigure)):
        if i + 1 < len(currentFigure):
            my_canvas.print_line(currentFigure[i].x,currentFigure[i].y,currentFigure[i+1].x, currentFigure[i+1].y)
    
    



#Выбор метода закраски
def set_delay(value):
    global delay
    delay = value


def clear_canvas():
    allFigures.clear()
    currentFigure.clear()
    listPoint_scroll.delete(0, tk.END)
    my_canvas.clear_all()


'''---------------- ИНТЕРФЕЙС --------------------'''
#Создание окна
win = tk.Tk()
win.title('Лабораторная №5')    #Название
size_win_str = f"{WIN_WIDTH}x{WIN_HEIGHT}+{WIN_X}+{WIN_Y}"
win.geometry(size_win_str)    #Размеры окна
win.config(bg = TK_BG)    #Добавление розового фона
win.resizable(False, False)    #Запрет на изменение размеров

#Создание меню
menu = tk.Menu(win)
menu.add_command(label="Вариант", command = show_var)
menu.add_command(label="Справка", command = show_info)
menu.add_command(label="Выйти", command=win.destroy)
win.config(menu=menu)

#Создание холста canvas
my_canvas = my_draw_class(win)
my_canvas.canvas.bind("<Button-1>", add_point_figure_onClick)
my_canvas.canvas.bind("<Button-3>", lambda event: close_figure())
my_canvas.canvas.bind("<Motion>", lambda event: my_canvas.show_coord(event))

drawing = 0
ctrl_pressed = 0
currentFigure = []
allFigures = []

#Для радиокнопок
var = tk.IntVar()  #класс позволяет хранить целочисленные значения
var.set(0)

#Цвет 
fill_color_x = CANVAS_WIDTH + 10
fill_color_y = 10
deltay = 0
deltax = 0
delta = 0

choose_color_lable = Label(win, text = "Выбор цвета:", justify = tk.CENTER, background= TK_BG,foreground = "black", font = "Arial 15")
choose_color_lable.place(x = fill_color_x + 180, y = fill_color_y)
fill_color_y += 40

fill_color_lable = Label(win, text = "Цвет закраски:", justify = tk.CENTER, background= TK_BG,foreground = "black", font = "Arial 14")
fill_color_lable.place(x = fill_color_x, y = fill_color_y)

deltay += 30
deltax += 142

choose_fill_color = tk.Button(win, text = 'Выбор цвета',font = 'Arial 13', bg = 'blue',foreground='white',command = lambda: my_canvas.choose_fill_color(cur_fill_color))
choose_fill_color.place(x = fill_color_x + 160, y = fill_color_y)
deltax += 160


#Текущий цвет отрезка
cur_fill_color_lable = Label(win, text = "Текущий цвет -", justify = tk.CENTER, background= TK_BG,foreground = "black", font = "Arial 13")
cur_fill_color_lable.place(x = fill_color_x + deltax, y = fill_color_y)
deltax += 130
cur_fill_color = tk.Button(win, width = 2, height = 1)
cur_fill_color.config(bg = my_canvas.fill_color[1], activebackground = my_canvas.fill_color[1])
cur_fill_color.place(x = fill_color_x + deltax, y = fill_color_y)

#Цвет фона
border_color_x = CANVAS_WIDTH + 30
border_color_y = fill_color_y + 60
deltay = 0
deltax = 0
delta = 0
border_color_lable = Label(win, text = "Цвет границы:", justify = tk.CENTER, background= TK_BG,foreground = "black", font = "Arial 14")
border_color_lable.place(x = border_color_x, y = border_color_y)

choose_border_color = tk.Button(win, text = 'Выбор цвета',font = 'Arial 13', bg = 'green',foreground='white',command = lambda: my_canvas.choose_border_color(cur_border_color))
choose_border_color.place(x = fill_color_x + 160, y = border_color_y)
deltax += 280

#Текущий цвет фона
cur_border_color_lable = Label(win, text = "Текущий цвет -", justify = tk.CENTER, background= TK_BG,foreground = "black", font = "Arial 13")
cur_border_color_lable.place(x = border_color_x + deltax, y = border_color_y)
deltax += 130
cur_border_color = tk.Button(win, width = 2, height = 1)
cur_border_color.config(bg = my_canvas.border_color[1], activebackground = my_canvas.border_color[1])
cur_border_color.place(x = border_color_x + deltax, y = border_color_y)

#Алгоритм построения
delay_x = border_color_x
delay_y = border_color_y + 50

delay_lable = Label(win, text = "Режим закраски:", justify = tk.CENTER,background = TK_BG ,foreground = "black", font = "Arial 14")
delay_lable.place(x = delay_x + 145, y = delay_y)

nodelay_radio = Radiobutton(win, text = "Без задержки", command = lambda: set_delay(False), variable = var, value = 0)
nodelay_radio.place(x = delay_x + 50, y = delay_y + 30)
nodelay_radio.config(background = TK_BG, foreground = "black", font = ("Arial", 13))
delay_radio = Radiobutton(win, text = "С задержкой", command = lambda: set_delay(True), variable = var, value = 1)
delay_radio.config(background = TK_BG, foreground = "black", font = ("Arial", 13))
delay_radio.place(x = delay_x + 300, y = delay_y + 30)

#Ввод точек
x_center = border_color_x + 135
y_center = delay_y + 80
center_lable = Label(win, text = "Добавление точек:", justify = tk.CENTER, background= TK_BG,foreground = "black", font = "Arial 14")
center_lable.place(x = x_center, y = y_center)

x_x = CANVAS_WIDTH + 3
y_y = y_center + 60

lab_x= tk.Label(win, text = 'x:', font = 'Arial 13 bold',background=TK_BG, foreground='black')
ent_x = tk.Entry(win, font = 'Arial 13', justify = 'left', width=10)
lab_x.place(x = x_x, y = y_y)
ent_x.place(x = x_x + 25, y = y_y)

x_y =  x_x 

lab_y= tk.Label(win, text = 'y:', font = 'Arial 13 bold',background=TK_BG, foreground='black')
ent_y = tk.Entry(win, font = 'Arial 13', justify = 'left', width=10)
lab_y.place(x = x_y , y = y_y + 30)
ent_y.place(x = x_y + 25, y = y_y + 30)

#Кнопка добавления точки
x_but_add = x_x + 130
y_but_add = y_y -10
but_add = tk.Button(win, text = ' Добавить точку ',font = 'Arial 13', bg = 'blue',foreground='white', command = add_point_button)
but_add.place(x = x_but_add, y = y_but_add)

#Кнопка замкнуть фигуру
y_but_close = y_but_add + 40
but_close = tk.Button(win, text = 'Замкнуть фигуру',font = 'Arial 13', bg = 'blue',foreground='white',command=close_figure)
but_close.place(x = x_but_add, y = y_but_close)

#Таблица точек
scroll_x = x_but_add + 160
scroll_y = y_but_add
listPoint_scroll = tk.Listbox(font=("Consolas", 12))
listPoint_scroll.place(x=scroll_x, y=scroll_y, width=280, height=200)

#Построение мышью
x_mouse = x_x
y_mouse = y_but_close + 50
Mouse_info = tk.Label(win, bg=TK_BG, text="*Построение фигуры с помощью мыши:\nЛевая кнопка мыши - добавить точку\n Правая кнопка мыши - замкнуть фигуру",font = 'Arial 11', foreground= 'black')
Mouse_info.place(x = x_mouse, y = y_mouse)

#Закрасить
x_fill = CANVAS_WIDTH + 110
y_fill = scroll_y + 220
but_fill= tk.Button(win, text = 'Закрасить',width = 13, font = 'Arial 14', bg = 'green',foreground='white', command = fill_all_figures)
but_fill.place(x = x_fill, y = y_fill)

#Очистить
x_clear = x_fill + 165
y_clear = y_fill 
but_clear= tk.Button(win, text = 'Очистить',width = 13, font = 'Arial 14', bg = 'purple',foreground='white', command = clear_canvas)
but_clear.place(x = x_clear, y = y_clear)

# x_clear_p = x_fill + 165
# y_clear_p = y_fill + 30
# but_clear_p= tk.Button(win, text = 'Удалить последнюю точку',width = 13, font = 'Arial 14', bg = 'purple',foreground='white', command = delete_point)
# but_clear_p.place(x = x_clear_p, y = y_clear_p)

win.mainloop()
