// ================================== make clean ================================== //
// use lps;
const db = db.getSiblingDB("lps");  // установить текущую БД

db.group.deleteMany({});
db.sector.deleteMany({});
db.object.deleteMany({});
db.movement_rule.deleteMany({});
db.violation_log.deleteMany({});

// ================================== object ================================== //

db.object.insertMany([
  {
    _id: "obj_001",
    group_id: "grp_engineers",
    type: "person_tag",
    owner_name: "Иванов С.А.",
    device_uid: "DEV-ENG-001"
  },
  {
    _id: "obj_002",
    group_id: "grp_operators",
    type: "person_tag",
    owner_name: "Петрова И.Н.",
    device_uid: "DEV-OPR-002"
  },
  {
    _id: "obj_003",
    group_id: "grp_equipment",
    type: "equipment_tag",
    owner_name: "SMD Автомат #1",
    device_uid: "DEV-EQP-003"
  },
  {
    _id: "obj_004",
    group_id: "grp_visitors",
    type: "person_tag",
    owner_name: "Экскурсант Мария",
    device_uid: "DEV-VIS-004"
  },
  {
    _id: "obj_005",
    group_id: "grp_equipment",
    type: "equipment_tag",
    owner_name: "Осциллограф 2",
    device_uid: "DEV-CHM-005"
  }
]);

// ================================== group ================================== //

db.group.insertMany([
  {
    _id: "grp_engineers",
    name: "Инженеры",
    description: "Сотрудники технического отдела, допущенные ко всем зонам"
  },
  {
    _id: "grp_operators",
    name: "Операторы",
    description: "Линия производственного контроля, ограниченный доступ"
  },
  {
    _id: "grp_equipment",
    name: "Оборудование",
    description: "Метки, закреплённые на движущихся механизмах и автоматах"
  },
  {
    _id: "grp_visitors",
    name: "Посетители",
    description: "Временные гости предприятия, требуют сопровождения"
  },
  {
    _id: "grp_chem_robots",
    name: "Химические роботы",
    description: "Автоматизированные установки для спецпроцессов"
  }
]);

// ================================== sector ================================== //

db.sector.insertMany([
  {
    _id: "sect_soldering",
    name: "Паяльная зона",
    type: "рабочая зона",
    geometry: {
        start: { x: 0, y: 0, z: 0 },    // начальная точка
        size: { x: 3, y: 4, z: 3 }       // размеры: 3м в длину, 4м в ширину, 3м в высоту
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  },
  {
    _id: "sect_chem_zone",
    name: "Зона химобработки",
    type: "опасная зона",
    geometry: {
        start: { x: 3, y: 0, z: 0 },
        size: { x: 3, y: 4, z: 3 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  },
  {
    _id: "sect_smd_placement",
    name: "Участок установки компонентов",
    type: "автоматизированная зона",
    geometry: {
        start: { x: 5, y: 1, z: 3 },
        size: { x: 3, y: 3, z: 3 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  },
  {
    _id: "sect_storage",
    name: "Зона хранения",
    type: "склад",
    geometry: {
        start: { x: 6, y: 0, z: 0 },
        size: { x: 4, y: 4, z: 3 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  },
  {
    _id: "sect_common",
    name: "Общий проход",
    type: "проход",
    geometry: {
        start: { x: 0, y: 7, z: 0 },
        size: { x: 10, y: 3, z: 3 }
    },
    floor_number: 1,
    building_name: "Цех микроэлектроники №3"
  }
]);

// ================================== movement_rule ================================== //
// access_type:
//     1 — Строго запрещено
//     2 — Запрещено, но допустимо в крайних случаях
//     3 — Не рекомендуется

db.movement_rule.insertMany([
  {
    group_id: "grp_visitors",
    sector_id: "sect_smd_placement",
    access_type: 1,
    time_from: "00:00",
    time_to: "23:59",
    reason: "Посетителям строго запрещён доступ в зону установки компонентов"
  },
  {
    group_id: "grp_chem_robots",
    sector_id: "sect_chem_zone",
    access_type: 1,
    time_from: "13:00",
    time_to: "17:00",
    reason: "Во время активной химической обработки никто не должен находиться в зоне"
  },
  {
    group_id: "grp_engineers",
    sector_id: "sect_soldering",
    access_type: 3,
    time_from: "20:00",
    time_to: "08:00",
    reason: "Ночное пребывание инженеров в паяльной зоне не рекомендуется"
  },
  {
    group_id: "grp_equipment",
    sector_id: "sect_common",
    access_type: 2,
    time_from: "12:00",
    time_to: "13:00",
    reason: "Во время перерыва нежелательно движение оборудования в общем проходе"
  },
  {
    group_id: "grp_operators",
    sector_id: "sect_chem_zone",
    access_type: 2,
    time_from: "00:00",
    time_to: "23:59",
    reason: "Операторам доступ в зону химобработки ограничен — только в сопровождении инженера"
  }
]);

// ================================== violation_log ================================== //
db.violation_log.insertMany([
  {
    _id: "viol_001",
    object_id: "obj_001",
    sector_id: "sect_smd_placement",
    movement_rule_id: "rule_001",
    severity: 3,
    timestamp: ISODate("2025-05-29T14:45:00Z"),
    coords: {
      x: 3.2,
      y: 2.1,
      z: 3.8
    }
  },
  {
    _id: "viol_002",
    object_id: "obj_002",
    sector_id: "sect_chem_zone",
    movement_rule_id: "rule_002",
    severity: 2,
    timestamp: ISODate("2025-05-29T13:15:00Z"),
    coords: {
      x: 3.5,
      y: 1.2,
      z: 2.9
    }
  },
  {
    _id: "viol_003",
    object_id: "obj_003",
    sector_id: "sect_soldering",
    movement_rule_id: "rule_003",
    severity: 1,
    timestamp: ISODate("2025-05-28T22:30:00Z"),
    coords: {
      x: 1.0,
      y: 2.0,
      z: 1.0
    }
  },
  {
    _id: "viol_004",
    object_id: "obj_004",
    sector_id: "sect_soldering",
    movement_rule_id: "rule_003",
    severity: 1,
    timestamp: ISODate("2025-05-28T23:50:00Z"),
    coords: {
      x: 2.8,
      y: 3.1,
      z: 0.9
    }
  },
  {
    _id: "viol_005",
    object_id: "obj_004",
    sector_id: "sect_common",
    movement_rule_id: "rule_004",
    severity: 2,
    timestamp: ISODate("2025-05-29T12:10:00Z"),
    coords: {
      x: 6.5,
      y: 8.0,
      z: 0.0
    }
  },
  {
    _id: "viol_006",
    object_id: "obj_005",
    sector_id: "sect_chem_zone",
    movement_rule_id: "rule_005",
    severity: 2,
    timestamp: ISODate("2025-05-29T09:45:00Z"),
    coords: {
      x: 4.1,
      y: 0.5,
      z: 0.3
    }
  },
  {
    _id: "viol_007",
    object_id: "obj_001",
    sector_id: "sect_chem_zone",
    movement_rule_id: "rule_005",
    severity: 2,
    timestamp: ISODate("2025-05-29T18:20:00Z"),
    coords: {
      x: 5.2,
      y: 1.9,
      z: 1.0
    }
  },
  {
    _id: "viol_008",
    object_id: "obj_002",
    sector_id: "sect_smd_placement",
    movement_rule_id: "rule_001",
    severity: 3,
    timestamp: ISODate("2025-05-28T11:30:00Z"),
    coords: {
      x: 5.5,
      y: 1.5,
      z: 4.2
    }
  },
  {
    _id: "viol_009",
    object_id: "obj_003",
    sector_id: "sect_common",
    movement_rule_id: "rule_004",
    severity: 1,
    timestamp: ISODate("2025-05-29T12:25:00Z"),
    coords: {
      x: 9.0,
      y: 8.5,
      z: 0.1
    }
  },
  {
    _id: "viol_010",
    object_id: "obj_004",
    sector_id: "sect_chem_zone",
    movement_rule_id: "rule_005",
    severity: 3,
    timestamp: ISODate("2025-05-28T15:40:00Z"),
    coords: {
      x: 3.2,
      y: 1.7,
      z: 2.0
    }
  }
]);

