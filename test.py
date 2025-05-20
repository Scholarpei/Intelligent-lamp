from record import Record
from snap import is_Snap,Load
import os
import shutil

if __name__=='__main__':
    Load('pth/best_pth')
    print("counting")
    while True:
        Record("tmp/tmp.wav",debug=False)
        val=is_Snap("tmp/tmp.wav")
        if(val>0.9):
            # print(val)
            print("Snap!")
            # exit()
            continue
            for i in range(114514):
                filename="audios/un_snaps/un_snap{0}.wav".format(i)
                if(os.path.exists(filename)):
                    continue
                shutil.copy("tmp/tmp.wav",filename)
                break