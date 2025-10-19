ITERATIONS <- 100000000

calculate_pi <- function() {
    pi_approx <- 0.0
    sign <- 1.0
    
    for (i in 0:(ITERATIONS - 1)) {
        pi_approx <- pi_approx + sign / (2.0 * i + 1.0)
        sign <- -sign
    }
    
    pi_approx <- pi_approx * 4.0
    result <- pi_approx  # Em R não precisamos de 'volatile'
    
    return(result)
}

# Executar a função
pi_result <- calculate_pi()

