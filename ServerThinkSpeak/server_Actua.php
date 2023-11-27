<?php
// Paso 1: Obtener del formulario el modo ("Automatico" o "Manual")
$modo = $_POST['Modo'];

// Paso 2: Obtener el estado de la lámpara y el ventilador según el modo
if ($modo == "Manual") {
    $estadoLampara =  $_POST['lampara_estado'];
    $estadoVentilador = $_POST['ventilador_estado'];
} elseif ($modo == "Automatico") {
    $estadoLampara = "Automatico";
    $estadoVentilador = "Automatico";
}

// Paso 3: Realizar la conexión con la base de datos "reporte"
$host = "localhost";
$db = "id21371168_invernadero";
$user = "id21371168_root";
$password = "W3b0s1.0";

try {
    $pdo = new PDO("mysql:host=$host;dbname=$db", $user, $password);
} catch (PDOException $e) {
    echo "Error de conexión: " . $e->getMessage();
    die();
}

// Paso 4: Generar una consulta que obtenga los datos de temperatura e iluminación del último registro insertado
$query = "SELECT temperatura, iluminacion FROM reporte ORDER BY id_reporte DESC LIMIT 1";
$result = $pdo->query($query);

if ($result) {
    $row = $result->fetch(PDO::FETCH_ASSOC);
    $temperatura = $row['temperatura'];
    $iluminacion = $row['iluminacion'];
} else {
    $temperatura = "No disponible";
    $iluminacion = "No disponible";
}

// Paso 5: Retornar los resultados con un echo
echo json_encode(array(
    'temperatura' => $temperatura,
    'iluminacion' => $iluminacion,
    'estadoLampara' => $estadoLampara,
    'estadoVentilador' => $estadoVentilador
));
?>