// ================================== make clean ================================== //
// use lps;
const db = db.getSiblingDB("lps");  // установить текущую БД

db.group.deleteMany({});
db.sector.deleteMany({});
db.object.deleteMany({});
db.movement_rule.deleteMany({});
db.violation_log.deleteMany({});


// ================================== group ================================== //

let r_grp_001 = db.group.insertOne(
  {
    name: "Инженеры",
    description: "Сотрудники технического отдела, допущенные ко всем зонам"
  });
let r_grp_002 = db.group.insertOne(
  {
    name: "Операторы",
    description: "Линия производственного контроля, ограниченный доступ"
  });
let r_grp_003 = db.group.insertOne(
  {
    name: "Оборудование",
    description: "Метки, закреплённые на движущихся механизмах и автоматах"
  });
let r_grp_004 = db.group.insertOne(
  {
    name: "Посетители",
    description: "Временные гости предприятия, требуют сопровождения"
  });
let r_grp_005 = db.group.insertOne(
  {
    name: "Химические роботы",
    description: "Автоматизированные установки для спецпроцессов"
  });

let grp_engineers = r_grp_001.insertedId;
let grp_operators = r_grp_002.insertedId;
let grp_equipment = r_grp_003.insertedId;
let grp_visitors = r_grp_004.insertedId;
let grp_chem_robots = r_grp_005.insertedId;

// ================================== object ================================== //

let r_obj_001 = db.object.insertOne(
  {
    group_id: grp_engineers,
    type: "person_tag",
    owner_name: "Иванов С.А.",
    device_uid: "DEV-ENG-001"
  });
  let r_obj_002 = db.object.insertOne(
  {
    group_id: grp_operators,
    type: "person_tag",
    owner_name: "Петрова И.Н.",
    device_uid: "DEV-OPR-002"
  });
  let r_obj_003 = db.object.insertOne(
  {
    group_id: grp_equipment,
    type: "equipment_tag",
    owner_name: "SMD Автомат #1",
    device_uid: "DEV-EQP-003"
  });
  let r_obj_004 = db.object.insertOne(
  {
    group_id: grp_visitors,
    type: "person_tag",
    owner_name: "Экскурсант Мария",
    device_uid: "DEV-VIS-004"
  });
  let r_obj_005 = db.object.insertOne(
  {
    group_id: grp_equipment,
    type: "equipment_tag",
    owner_name: "Осциллограф 2",
    device_uid: "DEV-CHM-005"
  });

  let obj_001 = r_obj_001.insertedId;
  let obj_002 = r_obj_002.insertedId;
  let obj_003 = r_obj_003.insertedId;
  let obj_004 = r_obj_004.insertedId;
  let obj_005 = r_obj_005.insertedId;



// ================================== sector ================================== //

let r_sect_soldering = db.sector.insertOne(
  {
    name: "Паяльная зона",
    type: "рабочая зона",
    geometry: {
        start: { x: 0.0, y: 0.0, z: 0.0 },
        size: { x: 3.0, y: 4.0, z: 3.0 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  });

let r_sect_chem_zone = db.sector.insertOne(
  {
    name: "Зона химобработки",
    type: "опасная зона",
    geometry: {
        start: { x: 3.0, y: 0.0, z: 0.0 },
        size: { x: 3.0, y: 4.0, z: 3.0 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  });

let r_sect_smd_placement = db.sector.insertOne(
  {
    name: "Участок установки компонентов",
    type: "автоматизированная зона",
    geometry: {
        start: { x: 5.0, y: 1.0, z: 3.0 },
        size: { x: 3.0, y: 3.0, z: 3.0 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  });

let r_sect_storage = db.sector.insertOne(
  {
    name: "Зона хранения",
    type: "склад",
    geometry: {
        start: { x: 6.0, y: 0.0, z: 0.0 },
        size: { x: 4.0, y: 4.0, z: 3.0 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  });

let r_sect_common = db.sector.insertOne(
  {
    name: "Общий проход",
    type: "проход",
    geometry: {
        start: { x: 0.0, y: 7.0, z: 0.0 },
        size: { x: 10.0, y: 3.0, z: 3.0 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  });

let sect_soldering = r_sect_soldering.insertedId;
let sect_chem_zone = r_sect_chem_zone.insertedId;
let sect_smd_placement = r_sect_smd_placement.insertedId;
let sect_storage = r_sect_storage.insertedId;
let sect_common = r_sect_common.insertedId;

// ================================== movement_rule ================================== //
// access_type:
//     1 — Строго запрещено
//     2 — Запрещено, но допустимо в крайних случаях
//     3 — Не рекомендуется

let r_rule_001 = db.movement_rule.insertOne(
  {
    group_id: grp_visitors,
    sector_id: sect_smd_placement,
    access_type: 1,
    time_from: "00:00",
    time_to: "23:59",
    reason: "Посетителям строго запрещён доступ в зону установки компонентов"
  });

let r_rule_002 = db.movement_rule.insertOne(
  {
    group_id: grp_chem_robots,
    sector_id: sect_chem_zone,
    access_type: 1,
    time_from: "13:00",
    time_to: "17:00",
    reason: "Во время активной химической обработки никто не должен находиться в зоне"
  });

let r_rule_003 = db.movement_rule.insertOne(
  {
    group_id: grp_engineers,
    sector_id: sect_soldering,
    access_type: 3,
    time_from: "20:00",
    time_to: "08:00",
    reason: "Ночное пребывание инженеров в паяльной зоне не рекомендуется"
  });

let r_rule_004 = db.movement_rule.insertOne(
  {
    group_id: grp_equipment,
    sector_id: sect_common,
    access_type: 2,
    time_from: "12:00",
    time_to: "13:00",
    reason: "Во время перерыва нежелательно движение оборудования в общем проходе"
  });

let r_rule_005 = db.movement_rule.insertOne(
  {
    group_id: grp_operators,
    sector_id: sect_chem_zone,
    access_type: 2,
    time_from: "00:00",
    time_to: "23:59",
    reason: "Операторам доступ в зону химобработки ограничен — только в сопровождении инженера"
  });

let rule_001 = r_rule_001.insertedId;
let rule_002 = r_rule_002.insertedId;
let rule_003 = r_rule_003.insertedId;
let rule_004 = r_rule_004.insertedId;
let rule_005 = r_rule_005.insertedId;

// ================================== violation_log ================================== //
let r_viol_001 = db.violation_log.insertOne(
  {
    object_id: obj_001,
    sector_id: sect_smd_placement,
    movement_rule_id: rule_001,
    severity: 3,
    timestamp: ISODate("2025-05-29T14:45:00Z"),
    coords: {
      x: 3.2,
      y: 2.1,
      z: 3.8
    }
  });

let r_viol_002 = db.violation_log.insertOne(
  {
    object_id: obj_002,
    sector_id: sect_chem_zone,
    movement_rule_id: rule_002,
    severity: 2,
    timestamp: ISODate("2025-05-29T13:15:00Z"),
    coords: {
      x: 3.5,
      y: 1.2,
      z: 2.9
    }
  });

let r_viol_003 = db.violation_log.insertOne(
  {
    object_id: obj_003,
    sector_id: sect_soldering,
    movement_rule_id: rule_003,
    severity: 1,
    timestamp: ISODate("2025-05-28T22:30:00Z"),
    coords: {
      x: 1.0,
      y: 2.0,
      z: 1.0
    }
  });

let r_viol_004 = db.violation_log.insertOne(
  {
    object_id: obj_004,
    sector_id: sect_soldering,
    movement_rule_id: rule_003,
    severity: 1,
    timestamp: ISODate("2025-05-28T23:50:00Z"),
    coords: {
      x: 2.8,
      y: 3.1,
      z: 0.9
    }
  });

let r_viol_005 = db.violation_log.insertOne(
  {
    object_id: obj_004,
    sector_id: sect_common,
    movement_rule_id: rule_004,
    severity: 2,
    timestamp: ISODate("2025-05-29T12:10:00Z"),
    coords: {
      x: 6.5,
      y: 8.0,
      z: 0.0
    }
  });

let r_viol_006 = db.violation_log.insertOne(
  {
    object_id: obj_005,
    sector_id: sect_chem_zone,
    movement_rule_id: rule_005,
    severity: 2,
    timestamp: ISODate("2025-05-29T09:45:00Z"),
    coords: {
      x: 4.1,
      y: 0.5,
      z: 0.3
    }
  });

let r_viol_007 = db.violation_log.insertOne(
  {
    object_id: obj_001,
    sector_id: sect_chem_zone,
    movement_rule_id: rule_005,
    severity: 2,
    timestamp: ISODate("2025-05-29T18:20:00Z"),
    coords: {
      x: 5.2,
      y: 1.9,
      z: 1.0
    }
  });

let r_viol_008 = db.violation_log.insertOne(
  {
    object_id: obj_002,
    sector_id: sect_smd_placement,
    movement_rule_id: rule_001,
    severity: 3,
    timestamp: ISODate("2025-05-28T11:30:00Z"),
    coords: {
      x: 5.5,
      y: 1.5,
      z: 4.2
    }
  });

let r_viol_009 = db.violation_log.insertOne(
  {
    object_id: obj_003,
    sector_id: sect_common,
    movement_rule_id: rule_004,
    severity: 1,
    timestamp: ISODate("2025-05-29T12:25:00Z"),
    coords: {
      x: 9.0,
      y: 8.5,
      z: 0.1
    }
  });

let r_viol_010 = db.violation_log.insertOne(
  {
    object_id: obj_004,
    sector_id: sect_chem_zone,
    movement_rule_id: rule_005,
    severity: 3,
    timestamp: ISODate("2025-05-28T15:40:00Z"),
    coords: {
      x: 3.2,
      y: 1.7,
      z: 2.0
    }
  });

let viol_001 = r_viol_001.insertedId;
let viol_002 = r_viol_002.insertedId;
let viol_003 = r_viol_003.insertedId;
let viol_004 = r_viol_004.insertedId;
let viol_005 = r_viol_005.insertedId;
let viol_006 = r_viol_006.insertedId;
let viol_007 = r_viol_007.insertedId;
let viol_008 = r_viol_008.insertedId;
let viol_009 = r_viol_009.insertedId;
let viol_010 = r_viol_010.insertedId;
