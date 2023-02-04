#include "romserializer.h"
#include <cstdint>
#include <QFile>

#include <iomanip>
#include <sstream>

#include <engine_core.h>
#include <QDebug>

#include "behaviorcompiler.h"

std::string uint8_t_to_hex(uint8_t x) {
    std::stringstream stream;
    stream << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)x;
    return stream.str();
}

#define MAX_ENTITIES 32
#define ENTITY_SLOTS 32

;
#pragma pack(push, r1, 1)

struct sprite_load_cmd {
    uint16_t gfx_addr;
    uint8_t gfx_bank;
    uint8_t gram_bank;
    uint8_t offset;
};

struct scene_header {
    uint8_t ent_slots_count;
    uint16_t ent_slots_addr;
    uint8_t ent_slots_bank;
    uint8_t entities_count;
    uint16_t entities_addr;
    uint8_t entities_bank;
    uint8_t sprite_load_count;
    uint16_t sprite_load_cmds;
    uint8_t sprite_load_bank;
};

struct SoA_Entities {
    uint8_t vx[MAX_ENTITIES];
    uint8_t vy[MAX_ENTITIES];
    uint8_t frame[MAX_ENTITIES];
    uint8_t slot[MAX_ENTITIES];
    uint8_t hp[MAX_ENTITIES];
    uint8_t state[MAX_ENTITIES];
};

struct SoA_EntSlots {
    uint8_t type[ENTITY_SLOTS];
    uint8_t gram_bank[ENTITY_SLOTS];
    uint8_t offset[ENTITY_SLOTS];
    uint8_t updater_addr_msb[ENTITY_SLOTS];
    uint8_t updater_addr_lsb[ENTITY_SLOTS];
    uint8_t updater_bank[ENTITY_SLOTS];
    uint8_t frame_table_addr_msb[ENTITY_SLOTS];
    uint8_t frame_table_addr_lsb[ENTITY_SLOTS];
    uint8_t frame_table_bank[ENTITY_SLOTS];
};

#pragma pack(pop, r1)

RomSerializer::RomSerializer(GTProject& myProject)
{
    QByteArray banks[256];
    bool bank_used[256];

    for(int i = 128; i < 256; ++i) {
        banks[i] = QByteArray();
        bank_used[i] = false;
    }

    vector<QByteArray> compressed_sprites;
    vector<QByteArray> frame_datas;

    uint16_t sprites_offset = 0x8000;
    uint8_t sprites_bank = 0xFE;
    uint16_t sprite_data_offsets[myProject.sprites.size()];
    uint8_t sprite_data_banks[myProject.sprites.size()];
    uint16_t frame_data_offsets[myProject.sprites.size()];
    uint8_t frame_data_banks[myProject.sprites.size()];
    int sprite_index = 0;
    for(auto& sprite : myProject.sprites) {
        compressed_sprites.push_back(sprite.getCompressedSheet());
        qsizetype compressed_size = compressed_sprites.back().size();
        if((sprites_offset + compressed_size) > 0xC000) {
            sprites_offset = 0x8000;
            --sprites_bank;
        }
        sprite_data_offsets[sprite_index] = sprites_offset;
        sprite_data_banks[sprite_index] = sprites_bank;
        bank_used[sprites_bank] = true;
        banks[sprites_bank].append(compressed_sprites.back());
        sprites_offset += compressed_size;

        QByteArray current_frame_data = sprite.getRomFrames();
        frame_datas.push_back(current_frame_data);
        int frames_size = current_frame_data.size();
        if((sprites_offset + frames_size) > 0xC000) {
            sprites_offset = 0x8000;
            --sprites_bank;
        }
        frame_data_offsets[sprite_index] = sprites_offset;
        frame_data_banks[sprite_index] = sprites_bank;
        bank_used[sprites_bank] = true;
        banks[sprites_bank].append(frame_datas.back());
        sprites_offset += frames_size;
        ++sprite_index;
    }

    BehaviorCompiler::PrepareToBuild();
    for(auto& behavior : myProject.behaviors) {
        if(!behavior.Compile(myProject.projectRoot)) {
            qDebug() << "couldn't compile" << behavior.name.c_str();
        }
    }
    for(auto& scene : myProject.scenes) {
        for(auto& slot : scene.entitySlots) {
            slot.behaviors->Compile(myProject.projectRoot);
        }
    }
    --sprites_bank;
    uint8_t behaviors_bank = sprites_bank;
    if(!BehaviorCompiler::RunLinker()) {
        qDebug() << "couldn't run linker";
    }
    map<string, int> behaviorsMap = BehaviorCompiler::ParseMap();
    bank_used[behaviors_bank] = true;

    QFile behaviors_bin(BehaviorCompiler::GetBinPath().string().c_str());
    behaviors_bin.open(QIODevice::ReadOnly);
    banks[behaviors_bank] = behaviors_bin.readAll();
    behaviors_bin.close();

    uint8_t gram_banks_by_sprite[myProject.sprites.size()];
    for(GTScene& scene : myProject.scenes) {
        uint16_t data_offset = 0x8000;
        uint8_t data_bank = 0x80;
        bank_used[data_bank] = true;
        scene_header sh;
        SoA_EntSlots scene_slots;
        SoA_Entities scene_entities;

        bool sprite_loaded[myProject.sprites.size()];
        for(int i = 0; i < myProject.sprites.size(); ++i) {
            sprite_loaded[i] = false;
        }
        vector<sprite_load_cmd> loadcmds;
        int slot_index = 0;
        int sprites_used = 0;
        for(GTEntitySlot& entslot : scene.entitySlots) {
            if(!sprite_loaded[entslot.sprite_id]) {
                sprite_load_cmd cmd;
                cmd.gfx_addr = sprite_data_offsets[entslot.sprite_id];
                cmd.gfx_bank = sprite_data_banks[entslot.sprite_id];
                cmd.gram_bank = 0x07 & sprites_used; //TODO allocate gram_bank better since this only supports 8 loads
                cmd.offset = 0;
                gram_banks_by_sprite[entslot.sprite_id] = cmd.gram_bank;
                loadcmds.push_back(cmd);
                ++sprites_used;
            } else {
                sprite_loaded[entslot.sprite_id] = true;
            }

            scene_slots.type[slot_index] = 0; //TODO set to actual specified type
            scene_slots.gram_bank[slot_index] = gram_banks_by_sprite[entslot.sprite_id];
            scene_slots.offset[slot_index] = 0;
            scene_slots.frame_table_addr_msb[slot_index] = (frame_data_offsets[entslot.sprite_id] & 0xFF00) >> 8;
            scene_slots.frame_table_addr_lsb[slot_index] = frame_data_offsets[entslot.sprite_id] & 0x00FF;
            scene_slots.frame_table_bank[slot_index] = frame_data_banks[entslot.sprite_id];

            if(entslot.behaviors->rowCount() == 0) {
                scene_slots.updater_addr_msb[slot_index] = 0x80;
                scene_slots.updater_addr_lsb[slot_index] = sizeof(scene_header); //assuming RTS placed right after scene header
                scene_slots.updater_bank[slot_index] = 0x80;
            } else {
                uint16_t updater_addr = behaviorsMap[entslot.behaviors->GenName()];
                scene_slots.updater_addr_msb[slot_index] = (updater_addr & 0xFF00) >> 8;
                scene_slots.updater_addr_lsb[slot_index] = updater_addr & 0x00FF;
                scene_slots.updater_bank[slot_index] = behaviors_bank;
            }

            ++slot_index;
        }

        int ent_index = 0;
        for(GTEntity& ent : scene.entities) {
            scene_entities.vx[ent_index] = ent.vx;
            scene_entities.vy[ent_index] = ent.vy;
            scene_entities.frame[ent_index] = ent.frame;
            scene_entities.hp[ent_index] = ent.hp;
            scene_entities.slot[ent_index] = ent.slot;
            scene_entities.state[ent_index] = ent.state;
            ++ent_index;
        }


        data_offset += sizeof(scene_header);

        //putting instant return updater after scene header
        ++data_offset;

        sh.ent_slots_count = scene.entitySlots.size();
        sh.ent_slots_addr = data_offset;
        sh.ent_slots_bank = data_bank;
        data_offset += sizeof(SoA_EntSlots);

        sh.entities_count = scene.entities.size();
        sh.entities_addr = data_offset;
        sh.entities_bank = data_bank;
        data_offset += sizeof(SoA_Entities);

        sh.sprite_load_count = loadcmds.size();
        sh.sprite_load_cmds = data_offset;
        sh.sprite_load_bank = data_bank;
        //data_offset += sizeof(sprite_load_cmd) * loadcmds.size();

        QByteArray sceneData;
        sceneData.append((const char*)&sh, sizeof(scene_header));
        sceneData.append(0x60); //ReTurn from Subroutine
        sceneData.append((const char*)&scene_slots, sizeof(SoA_EntSlots));
        sceneData.append((const char*)&scene_entities, sizeof(SoA_Entities));
        sceneData.append((const char*)loadcmds.data(), sizeof(sprite_load_cmd) * sh.sprite_load_count);

        sceneData.resize(16384, 0xFF);
        banks[data_bank].swap(sceneData);
    }

    std::filesystem::create_directory((myProject.projectRoot / path("rom")));
    std::filesystem::create_directory((myProject.projectRoot / path("rom_parts")));

    QFile romFile((myProject.projectRoot / path("rom") / path(myProject.name)).replace_extension(".gtr").string().c_str());
    romFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QByteArray bank_filler(16384, 0xFF);

    banks[255].append((const char*) bin_core_gtr_bankFF, bin_core_gtr_bankFF_len);
    bank_used[255] = true;

    for(int i = 128; i < 256; ++i) {
        if(bank_used[i]) {
            assert(banks[i].size() <= 16384);
            banks[i].resize(16384, 0xFF);
            string aaaaa = (myProject.projectRoot / path("rom_parts") / path(myProject.name)).replace_extension(".gtr").string() + ".bank" + uint8_t_to_hex(i);
            QFile bankFile(aaaaa.c_str());
            bankFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            bankFile.write(banks[i]);
            romFile.write(banks[i]);
            bankFile.close();
        } else {
            romFile.write(bank_filler);
        }
    }
    romFile.close();
}
