import can
import time
import tkinter as tk
from tkinter import filedialog as fd
import threading


def flash_over_can(
    abort_event: threading.Event,
    file_name: str
) -> None:
    # Check for valid file_name
    if file_name == '':
        print('Cant open file with empty name')
        return

    # Start
    print('Flashing started')

    # Open file
    with open(file_name, 'rb') as flash_file:
        flash_content: bytes = flash_file.read()
        flash_idx: int = 0
        flash_byte_count: int = len(flash_content)
        print(flash_byte_count)
        print(hex(flash_byte_count))

        # Open can bus
        with can.Bus(channel=0, bitrate=1_000_000, interface='vector') as bus:
            # Define CAN message ids
            CAN_MSG_ID_FLASH_INIT: int = 0x100
            CAN_MSG_ID_FLASH_DATA: int = 0x101
            CAN_MSG_ID_FLASH_ACK: int = 0x102
            CAN_MSG_ID_FLASH_FIN: int = 0x103

            # Convert flash_byte_count to can message content
            tx_data = [0] * 8
            for byte_idx in range(8):
                tx_data[7 - byte_idx] = (flash_byte_count >> (8 * byte_idx)) & 0xFF

            # Create init message
            tx_msg = can.Message(
                arbitration_id=CAN_MSG_ID_FLASH_INIT,
                data=tx_data,
                is_extended_id=False,
                is_rx=False
            )

            # State variables
            tx_ready: bool = False
            flash_finished: bool = False

            # Define can callback function
            def on_can_rx(msg: can.Message) -> None:
                nonlocal tx_ready
                nonlocal flash_finished
                msg_id: int = msg.arbitration_id
                if msg_id == CAN_MSG_ID_FLASH_ACK:
                    tx_ready = True
                    return
                elif msg_id == CAN_MSG_ID_FLASH_FIN:
                    flash_finished = True
                    return
                print('No acknowledgement from controller')
                tx_ready = False
                return

            # Create notifier
            notifier = can.Notifier(bus=bus, listeners=[on_can_rx])

            # Communication
            bus.send(tx_msg)
            while not flash_finished:
                if abort_event.is_set():
                    print('Flashing has been aborted')
                    return
            
                if tx_ready:
                    # Construct next message
                    tx_msg.arbitration_id = CAN_MSG_ID_FLASH_DATA
                    for data_idx in range(8):
                        tx_msg.data[data_idx] = flash_content[flash_idx]
                        flash_idx += 1

                    # Send message
                    bus.send(tx_msg)
            
                # Delay for bus reliability
                time.sleep(0.001)

            # End
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
    window.title('BrsCanFlasher')

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

    def on_flash_clicked() -> None:
        print('Clicked flash button')

        file_name = fd.askopenfilename()
        
        print(file_name)
        print(ctrlVar.get())

        flash_thread = threading.Thread(
            target=flash_over_can, 
            args=(abort_event, file_name,))
        flash_thread.start()

    flashBtn = tk.Button(
        text='Flash', 
        width=20, 
        height=1,
        command=on_flash_clicked).pack()
    
    # Main loop
    window.mainloop()

    # End
    abort_event.set()


if __name__ == '__main__':
    main()
