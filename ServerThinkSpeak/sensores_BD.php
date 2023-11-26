<?php
    // Comprobar el valor del POST
    $temperatura = $_POST['temperatura'] ?? null;
    $iluminacion = $_POST['iluminacion'] ?? null;
    
    // if ($temperatura !== null) {
    //     if ($temperatura > 50) {
    //         echo "1"; // Si la temperatura es mayor que 50, enviar "1"
    //     } else {
    //         echo "0"; // Si la temperatura es menor o igual a 50, enviar "0"
    //     }
    // }

    echo "temperatura=" . $temperatura . "&iluminacion=" . $iluminacion;
?>
