from multiprocessing import Process, Queue

def origem(fila1: Queue):
    for i in range(10):
        msg = "Teste " + str(i)
        # envia mensagem para a ponte pela fila 1
        print("Mensagem enviada:", msg)
        fila1.put(msg)


def ponte(fila1: Queue, fila2: Queue):
    for i in range(10):
        # recebe mensagem na fila 1
        # reenvia mensagem na fila 2
        msg = fila1.get()
        fila2.put(msg)
        print("Mensagem encaminhada:", msg)


def destino(fila2: Queue):
    for i in range(10):
        # recebe mensagem na fila 2
        msg = fila2.get()
        print("Mensagem recebida:", msg)


if __name__ == "__main__":
    fila1 = Queue()
    fila2 = Queue()
    o = Process(target=origem, args=(fila1,))
    p = Process(target=ponte, args=(fila1, fila2))
    d = Process(target=destino, args=(fila2,))

    processes = [o, p, d]
    queues = [fila1, fila2]

    # executa os processos
    for process in processes:
        process.start()

    # aguarda conclusão
    for process in processes:
        process.join()

    # libera as filas
    for queue in queues:
        queue.close()
        queue.join_thread()