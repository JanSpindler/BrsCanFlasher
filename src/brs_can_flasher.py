import can
import time
import tkinter as tk
from tkinter import filedialog as fd
import threading


def flash_over_can(abort_event: threading.Event) -> None:
    print('Flashing started')
    for i in range(32):
        if abort_event.is_set():
            print('Flashing has been aborted')
            return
        print(i)
        time.sleep(1)
    print('Flashing ended')


def flash_over_can1() -> None:
    with can.Bus(channel=0, bitrate=1_000_000, interface='vector') as bus:
        tx_msg = can.Message(
            arbitration_id=0x200, 
            data=[0, 25, 0, 1, 3, 1, 4, 1], 
            is_extended_id=False,
            is_rx=False
        )

        buffered_reader = can.BufferedReader()
        printer = can.Printer()

        notifier = can.Notifier(bus=bus, listeners=[buffered_reader, printer])

        while True:
            time.sleep(2.0)
            bus.send(tx_msg)
            print(tx_msg)


def main() -> None:
    print('General Kenobi')

    # Window
    window = tk.Tk()
    window.resizable(False, False)
    window.geometry('300x150')

    # Controller menu
    controllers = ['AMC', 'MBC', 'TMC']

    ctrlVar = tk.StringVar(window)
    ctrlVar.set(controllers[0]) # default value

    ctrlMenu = tk.OptionMenu(
        window,
        ctrlVar,
        *controllers).pack()

    # Flash button
    abort_event = threading.Event()
    flash_thread = threading.Thread(
            target=flash_over_can, 
            args=(abort_event,))

    def on_flash_clicked() -> None:
        print('Clicked flash button')
        fileName = fd.askopenfilename()
        print(fileName)
        print(ctrlVar.get())
        flash_thread.start()

    flashBtn = tk.Button(
        text='Flash', 
        width=20, 
        height=1,
        command=on_flash_clicked).pack()
    
    # Main loop
    window.mainloop()

    # End
    if flash_thread.is_alive():
        abort_event.set()
        flash_thread.join()


if __name__ == '__main__':
    main()
