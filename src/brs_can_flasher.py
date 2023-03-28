import can


def main():
    print('General Kenobi')
    
    with can.Bus(channel=0, bitrate=1_000_000, interface='vector') as bus:
        msg = can.Message(
            arbitration_id=0x200, 
            data=[0, 25, 0, 1, 3, 1, 4, 1], 
            is_extended_id=False
        )
        bus.send(msg)


if __name__ == '__main__':
    main()
