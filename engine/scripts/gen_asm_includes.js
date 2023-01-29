const fs = require('fs');

const mapFileContents = fs.readFileSync(process.argv[2],{encoding:'utf8', flag:'r'});

const sections = mapFileContents.split('\r\n\r\n');

const exportsListByName = sections[2].split(/-+/)[1].trim();

const exportCells = exportsListByName.split(/\s+/);
const engine_exports = [];

let entities_addr = "";

for(var i = 0; i < exportCells.length; i+=3) {
    const name = exportCells[i];
    const addr = exportCells[i+1];
    engine_exports.push({
        name, addr
    });
    if(name == "Entities") {
        entities_addr = addr.slice(-4);
    }
}

const defines = engine_exports.map((ex) => `${ex.name} = $${ex.addr.slice(-4)}`).join('\r\n');

console.log(';;;engine.inc - contains exported memory offsets from JukeboxGT engine');
console.log(defines);
console.log();
console.log(".struct ENTITIES");
console.log("\t.org $" + entities_addr);
console.log("vX .res 32");
console.log("vY .res 32");
console.log("Frame .res 32");
console.log("Slot .res 32");
console.log("HP .res 32");
console.log("State .res 32");
console.log(".endstruct");