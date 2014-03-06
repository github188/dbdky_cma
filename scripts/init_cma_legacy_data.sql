USE cma_legacy;

insert into `cma_legacy_devtypes` (`TypeID`, `TypeName`, `TypeComment`, `DataTableName`)
values
(1, 'qxhjjt', '气象环境静态', 'cma_lagacy_qxhjjt'),
(33, 'dxwdjt', '导线温度静态', 'cma_legacy_dxwdjt')

insert into `cma_legacy_device`
(`Netbasestation_ID`,`Voltage_Level`,`SN`,`Company`,`Contract`,`ServerInfo`,`Line`,`Tower_ID`,`Location`, `SIM`, `DeviceType_ID`, `ServerIP`, `DeviceIP`)
values
(),

