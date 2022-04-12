


from tkinter import *
from tkinter import filedialog

textv = ""
timedur = 100

fileloch = "D:/CODE/GIT/seproj/transportcpp/dump/dump2.txt"

# functions 
def openFile():
    global textv
    file_cont = open(fileloch).read()
    if textv == file_cont:
        ws.after(timedur, openFile)
        return
    
    textv = file_cont
    
    txtarea.delete("1.0","end")
    
    txtarea.insert(END, file_cont)
    ws.after(timedur, openFile) 

def saveFile():
    #tf = filedialog.asksaveasfile(mode='w',title ="Save file",defaultextension=".txt")
    tf = open(fileloch,"w")
    #tf.config(mode='w')

    pathh.insert(END, tf)
    data = str(txtarea.get(1.0, END))
    tf.write(data)
    
    tf.close()

ws = Tk()

ws.title("SE PROJECT GRP 8")
ws.geometry("400x500")
ws['bg']='#2a636e'

# adding frame
frame = Frame(ws)
frame.pack(pady=20)

# adding scrollbars 
ver_sb = Scrollbar(frame, orient=VERTICAL )
ver_sb.pack(side=RIGHT, fill=BOTH)

hor_sb = Scrollbar(frame, orient=HORIZONTAL)
hor_sb.pack(side=BOTTOM, fill=BOTH)

# adding writing space
txtarea = Text(frame, width=40, height=20)
txtarea.pack(side=LEFT)

# binding scrollbar with text area
txtarea.config(yscrollcommand=ver_sb.set)
ver_sb.config(command=txtarea.yview)

txtarea.config(xscrollcommand=hor_sb.set)
hor_sb.config(command=txtarea.xview)

# adding path showing box
pathh = Entry(ws)
pathh.pack(expand=True, fill=X, padx=10)
openFile()

# adding buttons 
Button(
    ws, 
    text="Open DB", 
    command=openFile
    ).pack(side=LEFT, expand=True, fill=X, padx=20)

Button(
    ws, 
    text="Save UPDATES", 
    command=saveFile
    ).pack(side=LEFT, expand=True, fill=X, padx=20)

Button(
    ws, 
    text="Exit", 
    command=lambda:ws.destroy()
    ).pack(side=LEFT, expand=True, fill=X, padx=20, pady=20)

ws.mainloop()
