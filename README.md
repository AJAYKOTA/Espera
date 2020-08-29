# Espera
ProducerConsumerproleam


// Producer will produce and cousumer will cousume with synchronization of a buffer and buffer size defineas 100.
// Until  Producer thread produces any data consumer thread can't comsume.
// Threads will use condition_variable to notify each other.
// we need mutex if we use condition_variable beacaue CV waits on mutex.
// it is one of way to slove producer consumer problem .

// packages and packages dimensions , i have taken as one variable.

// if package dimension between 20 to 60 as value , Package is labeled, otherwise it is discarded.
