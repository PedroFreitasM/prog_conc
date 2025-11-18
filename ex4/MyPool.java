// Nome: Pedro Freitas de Moura
// Código: Atualização do MyPool para calcular primos

import java.util.LinkedList;

class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;

    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        } 
    }

    public void execute(Runnable r) {
        synchronized(queue) {
            if (this.shutdown) return;
            queue.addLast(r);
            queue.notify();
        }
    }
    
    public void shutdown() {
        synchronized(queue) {
            this.shutdown=true;
            queue.notifyAll();
        }
        for (int i=0; i<nThreads; i++) {
          try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
    }

    private class MyPoolThreads extends Thread {
       public void run() {
         Runnable r;
         while (true) {
           synchronized(queue) {
             while (queue.isEmpty() && (!shutdown)) {
               try { queue.wait(); }
               catch (InterruptedException ignored){}
             }
             if (queue.isEmpty()) return;   
             r = (Runnable) queue.removeFirst();
           }
           try { r.run(); }
           catch (RuntimeException e) {}
         } 
       } 
    } 
}
 
class Hello implements Runnable {
   String msg;
   public Hello(String m) { msg = m; }

   //--metodo executado pela thread
   public void run() {
      System.out.println(msg);
   }
}

class Primo implements Runnable {
   int i;
   int n;
   public Primo(int num) { i = num; }
   public void run() {
     if (i<=1) {
	System.out.println(i + " não eh primo");
	return;
     }
     if (i==2){ 
	 System.out.println(i + " eh primo");
	 return;
     }
     if (i%2==0){ 
	 System.out.println(i + " não eh primo");
	 return;
     }
     for (n=3; n < Math.sqrt(i) + 1; n += 2){
       if(i%n==0){
         System.out.println(i + " não eh primo");
	 return;
       }
     }
     System.out.println(i + " eh primo");
     return;
  }
}
//Classe da aplicação (método main)
class MyPool {
    private static final int NTHREADS = 10;

    public static void main (String[] args) {
      FilaTarefas pool = new FilaTarefas(NTHREADS); 
      
      for (int i = 0; i < 25; i++) {
        final String m = "Hello da tarefa " + i;
        //Runnable hello = new Hello(m);
        //pool.execute(hello);
        Runnable primo = new Primo(i);
        pool.execute(primo);
      }

      pool.shutdown();
      System.out.println("Terminou");
   }
}
