import can
import time
from tkinter import *


def main() -> None:
    print('General Kenobi')
    
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



if __name__ == '__main__':
    main()
