/*!40000 ALTER TABLE `sd_cd` DISABLE KEYS */;


USE vmsdb;

/*INSERT INTO `sd_cd` (`OBJID`,`LinkedDevice`,`DeviceCode`,`Phase`,`JCLXBM`,`XLMC`,`SSZT`,`BJCSBMC`,`JCLXMC`,`ZIEDID`,`IEDID`) VALUES 
 //(1000020,'21M0601JC00000001','21M0601JC00000001','NULL','SD_JYZWH','NULL','NULL','NULL','绝缘子污秽','29M00000012252927','NULL\r');*/



DROP TABLE IF EXISTS `sd_jyzwh`;
CREATE TABLE `sd_jyzwh` (
  `OBJID` int(11) NOT NULL,
  `CDID` varchar(8) DEFAULT NULL,
  `AcquisitionTime` datetime DEFAULT NULL,
  `ESDD` decimal(9,4) DEFAULT NULL,
  `NSDD` decimal(9,4) DEFAULT NULL,
  `Daily_Max_Temperature` decimal(9,4)DEFAULT NULL,
  `Daily_Min_Temperature` decimal(9,4) DEFAULT NULL,
  `Daily_Max_Humidity` decimal(9,4) DEFAULT NULL,
  `Daily_Min_Humidity` decimal(9,4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=gbk;



