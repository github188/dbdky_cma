USE vmsdb;

DROP TABLE IF EXISTS `sd_fbjc`;
CREATE TABLE `sd_fbjc` (
	`OBJID` int(11) NOT NULL,
	`CDID` varchar(8) DEFAULT NULL,
	`AcquisitionTime` datetime DEFAULT NULL,
	`Tension` decimal(9,4) DEFAULT NULL,
	`Windage_Yaw_Angle` decimal(9,4) DEFAULT NULL,
	`Deflection_Angle` decimal(9,4) DEFAULT NULL,
	`Battery_Voltage` decimal(9,4) DEFAULT NULL,
	`Operation_Temperature` decimal(9,4) DEFAULT NULL,
	`Line_Temperature1` decimal(9,4) DEFAULT NULL,
	`Line_Temperature2` decimal(9,4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;