<?php
    // Paso 1: Obtener del formulario el modo ("Automatico" o "Manual")
    $modo = "Automatico"; // Establecer el modo predeterminado como "Automatico"
    if(isset($_POST['Modo'])){
        $modo = $_POST['Modo'];
    }

    // Paso 2: Obtener el estado de la lámpara y el ventilador según el modo
    $estadoLampara = "Automatico";
    $estadoVentilador = "Automatico";
    
    if ($modo == "Manual") {
        if(isset($_POST['lampara_estado'])){
            $estadoLampara = $_POST['lampara_estado'];
        }
        if(isset($_POST['ventilador_estado'])){
            $estadoVentilador = $_POST['ventilador_estado'];
        }
    }

    // Paso 3: Realizar la conexión con la base de datos "reporte"
    $host = "localhost";
    $db = "id21371168_invernadero";
    $user = "id21371168_root";
    $password = "W3b0s1.0";

    try {
        $pdo = new PDO("mysql:host=$host;dbname=$db", $user, $password);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    } catch (PDOException $e) {
        echo "Error de conexión: " . $e->getMessage();
        die();
    }

    // Paso 4: Generar una consulta que inserte los datos en la tabla estadoActuadores
    $fecha = date("Y-m-d");
    $hora = date("H:i:s");
    $stmt = $pdo->prepare("INSERT INTO estadoActuadores (lampara, ventilador, fecha, hora) VALUES (:lampara, :ventilador, :fecha, :hora)");
    $stmt->bindParam(':lampara', $estadoLampara);
    $stmt->bindParam(':ventilador', $estadoVentilador);
    $stmt->bindParam(':fecha', $fecha);
    $stmt->bindParam(':hora', $hora);

    try {
        $stmt->execute();
        echo "Datos insertados correctamente en la base de datos. $fecha-$hora";
    } catch (PDOException $e) {
        echo "Error al insertar datos: " . $e->getMessage();
    }
?>
