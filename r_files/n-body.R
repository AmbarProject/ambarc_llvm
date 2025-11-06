main <- function() {
  total <- 0
  n <- 50000000
  
  for (i in 0:(n - 1)) {
    a <- i * 3.14159
    b <- a / 2.71828
    c <- a + b - i
    
    temp <- as.integer(c)
    modulo <- temp - (temp %/% 1000) * 1000
    total <- total + modulo
  }
  
  return(total)
}

main()

