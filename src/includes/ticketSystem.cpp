//
// Created by Leon on 2021/4/21.
//

#include "ticketSystem.h"
//  ROLLBACK,BACKUP,REBUILD,COMMAND_LIST,STATUS
namespace LaMetropole {
    void (ticketSystem::*commandMap[22])(parser::PaperCup *) ={
            &ticketSystem::addUser, &ticketSystem::login, &ticketSystem::logout, &ticketSystem::queryProfile,
            &ticketSystem::modifyProfile, &ticketSystem::addTrain,
            &ticketSystem::releaseTrain, &ticketSystem::queryTrain, &ticketSystem::deleteTrain,
            &ticketSystem::queryTicket, &ticketSystem::queryTransfer,
            &ticketSystem::buyTicket, &ticketSystem::queryOrder, &ticketSystem::refundTicket, &ticketSystem::Clean,
            &ticketSystem::Exit, &ticketSystem::None, &ticketSystem::Rollback, &ticketSystem::Backup,
            &ticketSystem::Rebuild, &ticketSystem::CommandList, &ticketSystem::Status
    };

    ticketSystem::ticketSystem() : Nebula(&Libro) {
        //freopen("data/basic_6/1.in", "r", stdin);
        //freopen("output.out", "w", stdout);
        //std::ios::sync_with_stdio(false);
        std::fstream file("user_crystal.file", std::fstream::in);
        if (!file.is_open()) {
            Libro.initialise();
            Nebula.initialise();
        } else {
            Libro.initialise(true);
            Nebula.initialise(true);
        }
#ifdef back_up
        system("if [ ! -d \"environment\" ]; then \n mkdir environment \n fi");
        std::fstream backFile("environment/config.file", std::ios::in);
        if (!backFile.is_open()) {
            //total backup version | incremental backup version
            backFile.open("environment/config.file", std::ios::out);
            int tmp = -1;
            backFile.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            backFile.close();
            //todo
            backFile.open("environment/commands.file", std::ios::out);
            backFile.close();
        } else {
            int a, b;
            backFile.open("environment/config.file");
            backFile.read(reinterpret_cast<char *>(&a), sizeof(int));
            if (a >= 0) {
                backFile.seekg(sizeof(int) * (a + 1));
                backFile.read(reinterpret_cast<char *>(&b), sizeof(int));
            } else b = -1;
            Libro.Mathilda.setVersion(a, b);
            Libro.Sabine.setVersion(a, b);
            Nebula.Yoshikawa.setVersion(a, b);
            Nebula.Yuki.setVersion(a, b);
            Nebula.trainRecorder.setVersion(a, b);
            Nebula.Nancy.setVersion(a, b);
            Nebula.Jason.setVersion(a, b);
            Nebula.Arya.setVersion(a, b);
            backFile.close();
        }
        backFile.close();
#endif
        while (1) {
            LaMetropole::parser::PaperCup *cup = Apollo.listen();
            if (cup->keyType == parser::EXIT) {
                cout << "bye\n";
                delete cup;
                //fclose(stdin);
                //fclose(stdout);
                return;
            }
            (this->*commandMap[cup->keyType])(cup);
            delete cup;
        }
    }

    void ticketSystem::addUser(parser::PaperCup *cup) {
        cout << (Libro.add_user(cup) ? 0 : -1) << '\n';
    }

    void ticketSystem::Clean(parser::PaperCup *cup) {
        Libro.clear();
        Nebula.clear();
        cout << 0 << '\n';
    }

    void ticketSystem::Exit(parser::PaperCup *cup) {
        cout << "bye\n";
        exit(0);
    }

    void ticketSystem::addTrain(parser::PaperCup *cup) {
        cout << (Nebula.addTrain(cup) ? 0 : -1) << '\n';
    }

    void ticketSystem::buyTicket(parser::PaperCup *cup) {
        char t = Nebula.buyTicket(cup);
        switch (t) {
            case 'f':
                cout << "-1\n";
                break;
            case 'p':
                cout << "queue\n";
                break;
        }
    }

    void ticketSystem::deleteTrain(parser::PaperCup *cup) {
        cout << (Nebula.deleteTrain(cup) ? 0 : -1) << '\n';
    }

    void ticketSystem::login(parser::PaperCup *cup) {
        cout << (Libro.login(cup) ? 0 : -1) << '\n';
    }

    void ticketSystem::logout(parser::PaperCup *cup) {
        cout << (Libro.logout(cup) ? 0 : -1) << '\n';
    }

    void ticketSystem::refundTicket(parser::PaperCup *cup) {
        cout << (Nebula.refundTicket(cup) ? 0 : -1) << '\n';
    }

    void ticketSystem::releaseTrain(parser::PaperCup *cup) {
        cout << (Nebula.releaseTrain(cup) ? 0 : -1) << '\n';
    }

    void ticketSystem::queryTransfer(parser::PaperCup *cup) {
        if (!Nebula.queryTransfer(cup)) cout << "0\n";
    }

    void ticketSystem::queryTrain(parser::PaperCup *cup) {
        if (!Nebula.queryTrain(cup)) cout << "-1\n";
    }

    void ticketSystem::queryTicket(parser::PaperCup *cup) {
        if (!Nebula.queryTicket(cup)) cout << "0\n";
    }

    void ticketSystem::queryProfile(parser::PaperCup *cup) {
        if (!Libro.query_profile(cup)) cout << "-1\n";
    }

    void ticketSystem::queryOrder(parser::PaperCup *cup) {
        if (!Nebula.queryOrder(cup)) cout << "-1\n";
    }

    void ticketSystem::modifyProfile(parser::PaperCup *cup) {
        if (!Libro.modify(cup)) cout << "-1\n";
    }

    void ticketSystem::None(parser::PaperCup *cup) {}

    void ticketSystem::Rebuild(parser::PaperCup *cup) {
        int a, b;
        fstream file("environment/config.file");
        if (!file.is_open()) return;//todo
        file.read(reinterpret_cast<char *>(&a), sizeof(int));
        file.seekg(sizeof(int) * (a + 1));
        file.read(reinterpret_cast<char *>(&b), sizeof(int));
        file.close();
        Libro.Mathilda.rollBack(a, b);
        Libro.Sabine.rollBack(a, b);
        Nebula.Yoshikawa.rollBack(a, b);
        Nebula.Yuki.rollBack(a, b);
        Nebula.trainRecorder.rollBack(a, b);
        Nebula.Nancy.rollBack(a, b);
        Nebula.Jason.rollBack(a, b);
        Nebula.Arya.rollBack(a, b);
    }

    void ticketSystem::CommandList(parser::PaperCup *cup) {}

    void ticketSystem::Backup(parser::PaperCup *cup) {
        std::fstream backFile;
        int flag = 1, a, b;;
        if (cup && *cup->arg[0] == "t") {
            backFile.open("environment/config.file");
            backFile.read(reinterpret_cast<char *>(&a), sizeof(int));
            ++a;
            backFile.seekp(0);
            backFile.write(reinterpret_cast<char *>(&a), sizeof(int));
            backFile.seekp(sizeof(int) * (a + 1));
            b = -1;
            backFile.write(reinterpret_cast<char *>(&b), sizeof(int));
            backFile.close();
            string sysCom = "mkdir environment/totalBackup" + toString(a);
            system(sysCom.c_str());
            sysCom = "mkdir environment/totalBackup" + toString(a) + "/base";
            system(sysCom.c_str());
            Libro.Mathilda.setVersion(a, b);
            Libro.Sabine.setVersion(a, b);
            Nebula.Yoshikawa.setVersion(a, b);
            Nebula.Yuki.setVersion(a, b);
            Nebula.trainRecorder.setVersion(a, b);
            Nebula.Nancy.setVersion(a, b);
            Nebula.Jason.setVersion(a, b);
            Nebula.Arya.setVersion(a, b);
            flag = 0;
        } else {
            backFile.open("environment/config.file");
            backFile.read(reinterpret_cast<char *>(&a), sizeof(int));
            if (a == -1) return;
            backFile.seekg(sizeof(int) * (a + 1));
            backFile.read(reinterpret_cast<char *>(&b), sizeof(int));
            ++b;
            backFile.seekp(sizeof(int) * (a + 1));
            backFile.write(reinterpret_cast<char *>(&b), sizeof(int));
            backFile.close();
            string sysCom = "mkdir environment/totalBackup" + toString(a) + "/Inc" + toString(b);
            system(sysCom.c_str());
            Libro.Mathilda.setVersion(a, b);
            Libro.Sabine.setVersion(a, b);
            Nebula.Yoshikawa.setVersion(a, b);
            Nebula.Yuki.setVersion(a, b);
            Nebula.trainRecorder.setVersion(a, b);
            Nebula.Nancy.setVersion(a, b);
            Nebula.Jason.setVersion(a, b);
            Nebula.Arya.setVersion(a, b);
            flag = 1;
        }
        Libro.Mathilda.backup(flag);
        Libro.Sabine.backup(flag);
        Nebula.Yoshikawa.backup(flag);
        Nebula.Yuki.backup(flag);
        Nebula.trainRecorder.backup(flag);
        Nebula.Nancy.backup(flag);
        Nebula.Jason.backup(flag);
        Nebula.Arya.backup(flag);
    }

    void ticketSystem::Rollback(parser::PaperCup *cup) {
        int a = toLong(cup->arg[0]), b = toLong(cup->arg[1]);
        fstream file("environment/config.file");
        if (!file.is_open()) return;//todo
        int c;
        file.seekg(sizeof(int) * (a + 1));
        file.read(reinterpret_cast<char *>(&c), sizeof(int));
        file.close();
        if (b >= c) return;//todo
        Libro.Mathilda.rollBack(a, b);
        Libro.Sabine.rollBack(a, b);
        Nebula.Yoshikawa.rollBack(a, b);
        Nebula.Yuki.rollBack(a, b);
        Nebula.trainRecorder.rollBack(a, b);
        Nebula.Nancy.rollBack(a, b);
        Nebula.Jason.rollBack(a, b);
        Nebula.Arya.rollBack(a, b);
    }

    void ticketSystem::Status(parser::PaperCup *cup) {}
}