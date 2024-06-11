from tkinter import *
from tkinter import colorchooser
from math import *

from settings import *

class my_draw_class():

    #Цвет заливки
    fill_color = ((0.0, 0.0, 0.0),fill_color)
    #Цвет фона
    bg_color =  ((255.0, 255.0, 255.0),back_color)
    #Цвет границы
    border_color = ((128.0, 0.0, 0.0),border_color)
    canvas = None
    img = None

    #Конструктор
    def __init__(self, root):
        self.canvas = Canvas(root, width=CANVAS_WIDTH, height=CANVAS_HEIGHT, bg="lavender")
        self.canvas.place(x=0, y=0)

        self.img = PhotoImage(width=CANVAS_WIDTH, height=CANVAS_HEIGHT)
        self.canvas.create_image((CANVAS_WIDTH/2, CANVAS_HEIGHT/2), image=self.img, state="normal")
        self.img.put(self.bg_color[1], to=(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT))

    #Очистка холста
    def clear_all(self):
        self.canvas.delete('all')

        self.img = PhotoImage(width=CANVAS_WIDTH, height=CANVAS_HEIGHT)
        self.canvas.create_image((CANVAS_WIDTH/2, CANVAS_HEIGHT/2), image=self.img, state="normal")
        self.img.put(self.bg_color[1], to=(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT))

    #Инверсия цвета пикселя
    def reverse_pixel(self, x, y):
        color_pixel = self.img.get(round(x), round(y))
        if color_pixel == self.bg_color[0]:
            self.img.put(self.fill_color[1], (round(x), round(y)))
        elif color_pixel == self.fill_color[0]:
            self.img.put(self.bg_color[1], (round(x), round(y)))

    #Построение линии
    def print_line(self, xstart, ystart, xend, yend):
        self.canvas.create_line(xstart, ystart, xend, yend, fill=self.border_color[1],tags="border_line")
        #bresenham_int(self,[xstart,ystart], [xend,yend])

    #Высвечивания пикселя
    def print_pixel(self, x, y):
        self.canvas.create_line(round(x), round(y), round(x), round(y) + 1, width=1, fill=self.fill_color[1])  # , tags="user_line")
        #self.img.put(self.fill_color[1], (round(x), round(y)))

    #Высвечивание ребер фигуры (границ)
    def print_edges(self, x1, y1, x2, y2):
        self.canvas.create_line(x1, y1, x2, y2, fill=self.border_color[1] , tags="border_line")

    #Смена цвета закраски
    def choose_fill_color(self, cur_fill_color):
        temp = list(colorchooser.askcolor(title="Выбор цвета "))
        new_color = (tuple([int(i) for i in temp[0]]), temp[1])
        self.fill_color = new_color
        cur_fill_color["bg"] = self.fill_color[1]
        cur_fill_color["activebackground"] = self.fill_color[1]

    #Смена цвета границы
    def choose_border_color(self, cur_border_color):
        temp = list(colorchooser.askcolor(title="Выбор цвета"))
        new_color = (tuple([int(i) for i in temp[0]]), temp[1])
        self.border_color = new_color
        self.canvas.itemconfig("border_line", fill=self.border_color[1])
        #self.img.put(new_color[1], to=(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT))
        cur_border_color["bg"] = self.border_color[1]
        cur_border_color["activebackground"] = self.border_color[1]

    #Отображение координат при наведении мыши на холст
    def show_coord(self, event):
        self.canvas.delete("coordinates")
        self.canvas.create_text(event.x + 45, event.y - 15,  text=f"({event.x}, {event.y})", font="Arial 10",tags="coordinates")

#Аанализ знака переменной х
def sign(x):
    if x > 0:
        return 1
    elif x < 0:
        return -1
    else:
        return 0

#Построение линии алгоритмом Брезенхема
def  bresenham_int(canvas_class, start, stop):
    xn = start[0]
    yn = start[1]
    xk = stop[0]
    yk = stop[1]
    #Проверка на вырожденность отрезка (Если отрезок вырожден, то высвечивание точки и переход к концу функции)
    if xn == xk and yn == yk:
        canvas_class.print_pixel(int(xn), int(yn))
    else:
        #Вычисление приращений 
        dx = xk - xn
        dy = yk - yn
        #Вычисление шага изменения каждой координаты пиксела
        sx = sign(dx)
        sy = sign(dy)
        #Вычисление модулей приращения координат
        dy = abs(dy)
        dx = abs(dx)
        #Вычисление модуля тангенса угла наклона отрезка m
        #Анализ вычисленного значения m и обмен местами dX и dY при m > 1 
        #do_switch - флаг, определяющий факт обмена местами координат
        if dy > dx:
            dx, dy = dy, dx
            do_switch = 1
        else:
            do_switch = 0
        m = dy / dx
        
        e = 2 * dy - dx
        #Инициализация начальных значений координат текущего пикселя:
        x = xn
        y = yn

        x_prev = x
        y_prev = y
        i = 1
        while i < dx + 1:
            i += 1
            canvas_class.print_pixel(int(x), int(y))

            if e >= 0:
                if do_switch == 1:
                    x += sx
                else:
                    y += sy
                #Подсчет	нового	значения	ошибки	
                e -= 2 * dx  
            if e <= 0:
                if do_switch == 0:
                    x += sx
                else:
                    y += sy
                #Подсчет	нового	значения	ошибки	
                e += 2 * dy  
