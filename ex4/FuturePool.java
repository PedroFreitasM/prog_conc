// Nome: Pedro Freitas de Moura
// Código: Calculador de quantidade de primos usando Callable e Futures

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;


//classe runnable
 class MyCallable implements Callable<Long> {
  int numThreads;
  int n;
  int idCall;
  //construtor
  MyCallable(int nThreads, int num, int id) {
    numThreads = nThreads;
    n = num;
    idCall = id;
  }
  //método para calcular se um num é primo

  public int ehPrimo(Long n){
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n%2 == 0) return 0;
    for(i=3; i < Math.sqrt(n) + 1; i += 2){
      if (n%i == 0) return 0;
    }
    return 1;
  }
  public Long call() throws Exception {
    long s = 0;
    for (long i=idCall; i<=n; i+=numThreads) {
      s = s + ehPrimo(i);
    }
    return s;
  }
} 
 public class FuturePool  {
  private static final int num = 10;
  private static final int NTHREADS = 5;
  public static void main(String[] args) {
    ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
    List<Future<Long>> list = new ArrayList<Future<Long>>();
    for (int i = 0; i < NTHREADS; i++) {
      Callable<Long> worker = new MyCallable(NTHREADS, num, i);
      Future<Long> submit = executor.submit(worker);
      list.add(submit);
    }
    long sum = 0;
    for (Future<Long> future : list) {
      try {
        sum += future.get();
      } catch (InterruptedException e) {
        e.printStackTrace();
      } catch (ExecutionException e) {
        e.printStackTrace();
      }
    }
    System.out.println(sum);
    executor.shutdown();
  }
} 
