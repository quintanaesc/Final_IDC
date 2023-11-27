<?php
// Obtener los valores del POST
$temperatura = $_POST['temperatura'] ?? null;
$iluminacion = $_POST['iluminacion'] ?? null;
$lampara = $_POST['lampara'] ?? null;
$ventilador = $_POST['ventilador'] ?? null;

// Obtener la fecha y hora actual
$fecha = date('Y-m-d');
$hora = date('H:i:s');

// Credenciales de la base de datos
$host = "localhost";
$db = "id21371168_invernadero";
$user = "id21371168_root";
$password = "W3b0s1.0";

// Intentar establecer la conexión
try {
    $conn = new PDO("mysql:host=$host;dbname=$db", $user, $password);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Aquí puedes realizar operaciones con la base de datos si la conexión fue exitosa.
    // Por ejemplo, podrías insertar los valores de temperatura, iluminación, fecha y hora en una tabla.

    // Ejemplo de inserción de datos en una tabla llamada 'reporte'
    $stmt = $conn->prepare("INSERT INTO final (temperatura, iluminacion, lampara, ventilador, fecha, hora) VALUES (:temperatura, :iluminacion, :lampara, :ventilador, :fecha, :hora)");
    $stmt->bindParam(':temperatura', $temperatura);
    $stmt->bindParam(':iluminacion', $iluminacion);
    $stmt->bindParam(':lampara', $lampara);
    $stmt->bindParam(':ventilador', $ventilador);
    $stmt->bindParam(':fecha', $fecha);
    $stmt->bindParam(':hora', $hora);
    $stmt->execute();

    echo "Datos insertados correctamente en la base de datos.".$fecha."-".$hora;

} catch (PDOException $e) {
    echo "Error de conexión: " . $e->getMessage();
} 
?>