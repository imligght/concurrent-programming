from time import sleep
from random import randint
from threading import Thread, Semaphore

def produtor():
    global buffer
    for i in range(10):
        sleep(randint(0, 2))  # fica um tempo produzindo...
        item = "item " + str(i)

        # verifica se há lugar no buffer
        produtor_sem.acquire()
        buffer.append(item)
        consumidor_sem.release()

        print("Produzido %s (ha %i itens no buffer)" % (item, len(buffer)))


def consumidor():
    global buffer
    for i in range(10):
        # aguarda que haja um item para consumir
        consumidor_sem.acquire()
        item = buffer.pop(0)
        produtor_sem.release()

        print("Consumido %s (ha %i itens no buffer)" % (item, len(buffer)))
        sleep(randint(0, 2))  # fica um tempo consumindo...


buffer = []
tam_buffer = 3

# cria semáforos
produtor_sem = Semaphore(value=tam_buffer)
consumidor_sem = Semaphore(value=0)

produtor = Thread(target=produtor) 
consumidor = Thread(target=consumidor) 

produtor.start()
consumidor.start()

produtor.join()
consumidor.join() 