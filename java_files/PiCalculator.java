public class PiCalculator {
    public static double result; // variável global

    public static void main(String[] args) {
        final long ITERATIONS = 100_000_000L;
        double pi = 0.0;
        double sign = 1.0;

        for (long i = 0; i < ITERATIONS; i++) {
            pi += sign / (2.0 * i + 1.0);
            sign = -sign;
        }

        pi *= 4.0;
        result = pi; // prevenir otimização
    }
}

