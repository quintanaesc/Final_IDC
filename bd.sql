CREATE TABLE `id21371168_invernadero`.`reporte` (
    `id_reporte` INT NOT NULL AUTO_INCREMENT , 
    `temperatura` INT NOT NULL , 
    `iluminacion` VARCHAR(25) NOT NULL , 
    `fecha` DATE NOT NULL ,
    `hora` TIME NOT NULL , 
    PRIMARY KEY (`id_reporte`)) ENGINE = InnoDB;
CREATE TABLE `estadoActuadores` (
  `id_estado` int(11) NOT NULL,
  `lampara` varchar(15) NOT NULL,
  `ventilador` varchar(15) NOT NULL,
  `fecha` date NOT NULL,
  `hora` time NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
