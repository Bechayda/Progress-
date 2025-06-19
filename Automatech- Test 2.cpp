#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <string>

// Include custom headers for game screens and dialogue system
#include "TitleScreen.h"
#include "GameState.h"
#include "AboutScreen.h"
#include "PlayIntro.h"
#include "DialogueBox.h"
#include "BackgroundManager.h"
#include "SoundManager.h"
#include "LoadScreen.h"
#include "SaveManager.h"
#include "GameProgress.h"
#include "chapterManager.h"
#include "CharacterPortrait.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Escape from Biringan");

    // Load fonts
    sf::Font titleFont;
    if (!titleFont.loadFromFile("BlackDahlia.ttf")) {
        std::cerr << "Failed to load BlackDahlia.ttf\n";
        return -1;
    }

    sf::Font bodyFont;
    if (!bodyFont.loadFromFile("Railway.ttf")) {
        std::cerr << "Failed to load Railway.ttf\n";
        return -1;
    }

    // Initialize SoundManager 
    SoundManager& soundManager = SoundManager::getInstance();

    // Load sound effects
    if (!soundManager.loadSound("ECH1", "ECH1.ogg")) {
        std::cerr << "Failed to load ECH sound effect.\n";
    }

    // Play background music for the title screen
    if (!soundManager.playMusic("BeginMusic.ogg", true)) {
        std::cerr << "Failed to load background music.\n";
        return -1;
    }

    TitleScreen::GameState state = TitleScreen::GameState::TITLE;
    bool loadedFromSave = false;  // Track if "Load" was chosen

    while (window.isOpen()) {
        if (state == TitleScreen::GameState::TITLE) {
            TitleScreen titleScreen(window, titleFont, bodyFont, soundManager);
            state = titleScreen.run();
            loadedFromSave = false;  // Reset between transitions
        }
        else if (state == TitleScreen::GameState::ABOUT) {
            AboutScreen aboutScreen(window, titleFont, bodyFont);
            if (aboutScreen.run() == AboutScreen::AboutState::BACK) {
                state = TitleScreen::GameState::TITLE;
            }
        }
        else if (state == TitleScreen::GameState::LOAD) {
            LoadScreen loadScreen(window, titleFont, bodyFont);
            LoadScreen::LoadState loadState = loadScreen.run();
            if (loadState == LoadScreen::LoadState::BACK) {
                state = TitleScreen::GameState::TITLE;
            }
            else if (loadState == LoadScreen::LoadState::LOAD_SUCCESS) {
                GameProgress progress;
                if (SaveManager::loadProgress("savegame.dat", progress)) {
                    loadedFromSave = true;  // Set flag
                    state = TitleScreen::GameState::GAMEPLAY;
                }
                else {
                    std::cerr << "Failed to load saved game progress.\n";
                    state = TitleScreen::GameState::TITLE;
                }
            }
        }
        else if (state == TitleScreen::GameState::GAMEPLAY) {
            std::cout << "Starting gameplay...\n";

            BackgroundManager bgManager(window);
            DialogueBox dialogueBox(window, bodyFont);
            CharacterPortrait middlePortrait;

            if (!middlePortrait.load("mac.png")) {
                std::cerr << "Failed to load mysterious_character.png\n";
            }
            middlePortrait.setScale(0.6f, 0.6f);
            middlePortrait.setVisible(false);

            if (loadedFromSave) {
                GameProgress progress;
                if (SaveManager::loadProgress("savegame.dat", progress)) {
                    bgManager.setBackground(progress.backgroundImage);
                    std::vector<DialogueEntry> resume = {
                        {progress.dialogue, progress.speaker}
                    };
                    dialogueBox.startDialogue(resume);
                }
            }
            else {
                bgManager.setBackground("bgs.png");
                std::vector<DialogueEntry> dialogues = {
                    {"Dito ba talaga ang daan?", "???"},
                    {"Dapat talaga nagpasama ko. Masyadong madilim ang kalsada.", "???"},
                    {"Kaliwa ba o Kanan yung pupuntahan?", "???"},
                    {"“Apo, puntahan mo nga muna bahay nila Auntie Amelita mo. Bigyan mo sya nang \nnatirang maja.”", "Flashback(Mother)"},
                    {"Saan bahay ni Auntie?", "???"},
                    {"Tapat bahay nya sa municipal hall. Pangatlong bahay na may third floor.\nNaintindihan mo ?", "Flashback(Mother)"},
                    {"Opo!", "Flashback(???)"},
                    {"Sa tapat daw ng municipal hall....", "???"},
                    {"Malapit ka nang kumanan nang napansin mo na maraming nalabas na \nusok sa part na yun.", "???"},
                    {"Di mo ito ganong inisip at marami pa namang nagbebenta sa gabi.", "???"},
                    {"Try to make the smoke disperse using hand.", "???"},
                    {"Close your eyes.", "???"},
                    {"Ang dilim.", "???"},
                    {"Rub eyes.", "???"},
                    {"Dito ba talaga yun? Masyadong marangya parang lang kila tita.\nHindi kaya mali yung liko ko?", "???"},
                    {"Looks back", "???"},
                    {"TANGINA- nasan– may– nasaan na yung mga bahay dito?", "???"},
                    {"Step back", "???"},
                    {"Fuck", "???"},
                    {"Look Back.\nPan on the city of Biringan", "???"}
                };
                dialogueBox.startDialogue(dialogues);
            }

            bool showDialogue = true;
            bool fadeTriggered = false;
            bool fadeCompleted = false;
            bool chapter2Started = false;
            sf::Clock clock;

            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                        state = TitleScreen::GameState::TITLE;
                        break;
                    }
                    dialogueBox.handleInput(event);
                }

                float deltaTime = clock.restart().asSeconds();
                bgManager.update(deltaTime);
                dialogueBox.update();

                window.clear(sf::Color::Black);
                bgManager.draw();

                std::string currentDialogue = dialogueBox.getCurrentDialogue();

                if (showDialogue) {
                    if (currentDialogue == "You have entered the City of Biringan.") {
                        middlePortrait.setVisible(true);
                    }
                    else {
                        middlePortrait.setVisible(false);
                    }

                    // Center character portrait
                    if (middlePortrait.isVisible()) {
                        float centerX = window.getSize().x / 2.0f - middlePortrait.getGlobalBounds().width / 2.0f;
                        float posY = window.getSize().y * 0.2f;
                        middlePortrait.setPosition(centerX, posY);
                        middlePortrait.draw(window);
                    }

                    dialogueBox.draw();

                    if (currentDialogue == "Ang dilim." && !fadeTriggered) {
                        bgManager.setBackground("BlackBG.jpg");  
                        fadeTriggered = true;
                    }

                    if (currentDialogue == "Rub eyes." && !fadeCompleted) {
                        bgManager.setBackground("Birig.png");  
                        fadeCompleted = true;
                    }

                    if (currentDialogue == "Looks back") {
                        soundManager.playSound("ECH1");
                    }

                    if (currentDialogue == "Look Back.\nPan on the city of Biringan" && !chapter2Started) {
                        chapter2Started = true;

                        ChapterManager::transitionToChapter(window, bodyFont, "Chapter 2: Biringan");
                        ChapterManager::transitionToChapter(window, bodyFont, "TIME: 9:00 PM");
                        bgManager.setBackground("House.png");

                        std::vector<DialogueEntry> chapter2Dialogues = {
                            {"Mga naglalakihang mga gusali, maaayos ang mga daan at kay rami ang mga halaman\nang nakikita ko. Ibang-iba ito sa lugar na dapat na nakatayo duon. Parang isang\nbustling metropolitan kung sasabihin ng iba. Nararamdaman ko ang taas ng mga\nbuhok ko sa balahibo habang segu-segundo akong nakatayo sa gitna ng kalsadang. ", "???"},
                            {"nito. Tuloy tuloy ang galaw ng mga tao sa iba't ibang direksyon na parang normal lang\n ang nangyayari.Na parang walang kakaiba ang napapagmasdan ko. Nabaling\n ang tingin ko sa gitna ng daan. Marami ang nagtitipon at pinapaganda ang daanan.", "???"},
                            {"Naglalagay ng mga ilaw, bandana, balloons at mga kung ano-ano sa mga streetlights\nna katulad duon sa Jones Bridge. May mga tao na may dala-dalang mga basket na\npuno ng pagkain at mga halaman. Halatang may pagdiriwang ang nangyayari.\nMaraming naka-suot ng pormal at may mga iba na parang buwan ng wika ang atake " ,"???"},
                            {"Rinig na rinig ang mga paguusap, ang tawa ng paglalaro ng mga bata at ang mga\nkanta at ang kasiyahan.Pinagmamasdan ko sila, Mahahaba ang kanilang buhok\nat iba iba ang kanilang kulay, mga puti o kayumanggi ang balat, matatangkad\nat ang ga-ganda at gwapo na mapapabaling tingin ka ulit at iisipin kung ibang." , "???"},
                            {"mundo ba sila", "???"},
                            {"Ibang mundo?", "???"},
                            {"Magaganda", "???"},
                            {"Para akong nabuhusan ng tubig nang maintindihan ko ang lahat ng nangyayari.","???"},
                            {"Akala ko sabi-sabi lang ito. Mito kuno ng mga matatanda pang bigay takot.", "???"},
                            {"Napahakbang ako pabalik ng marinig ko ang tunog ng kampana. Nagsitahimikan\nang mga tao.", "???"},
                            //SOUND EFFECTS HERE  DUM DUM DUM DUM
                            {"Kada tunog ng kampana ay padagdag ng padagdag ang takot sa puso ko. Pinwersa\nko ang sarili ko na tignan ang nangyayari sa likod. Nakita kong binubuksan ang\npinto ng isang lumang gusali. Namangha ako sa Nakita kong display. Marangya lang\nang mad-describe ko sa nakita ko. Gintong mga karwahe, mga mamahaling kotse,", "???"},
                            {"mga karong ginto na may puting lace na punong puno ng pagkain, gamit at\nkayamanan.Mayroon rin silang mga bantay nakasuot ng mga lumang kasuotan at--", "???"},
                            {"UNGGOY BA YUN!? BA'T ANLAKI?", "???"},
                            {"Bigla may humugot sakin at--", "???"},
                            {"Ah, parade nga pala tas nasa gitna ako ng daanan.", "???"},
                            {"Bata, di ka ba aware sa nangyayari!? Sabi ng Ale.", "???"},
                            {"Isang matangkad na babae na na parang nasa bandang late 30s na. Mahaba at\nmaitim buhok at Morena at napakaganda. Pinoy na pinoy-- bilog ang mukha\nat pango. Pero bakit parang may mali sa features nya. ", "???"},
                            {"Nasa gitna ka nang daan, di mo ba napansin na palapit na sayo yung mga karwahe!?\nHindi mo ba Nakita na nagmake-way kami at gumilid? Pagalit na sagot ng Ale.", "???"},
                            {"Nararamdaman ko ang tingin ng iba.", "???"},
                            {"Hindi po, sorry po. Paumanhin ko. ", "???"},
                            {"Ay nako, mga bata nga naman ngayon. Iling nya na may kasamang tsk.", "???"},
                            {"San ka ba galing ha. Tignan mo suot mo! Nakapambahay lang. Akala mo di ka\npinalaki ng maayos ng magulang. Tuloy niya pa.", "???"},
                            {"Sorry po talaga. Sabi ko pero nasa iba na ang atensyon ko.", "???"},
                            {"Ngayon ko lang napansin ang mga hiyawan at ingay ng mga tao. Nagpatuloy ang\nparada at iba-ibang spetakulo ang lumabas sa gusali. May mga taong nakikinuod\nrin ang lumabas.", "???"},
                            {"Nak, umuwi ka na.", "???"},
                            {"'po?' Naputol ang aking panood at tumingin sa Ale.", "???"},
                            {"Kung ayaw mong manirahan ng habang buhay dito, umuwi ka na agad. Hanggang\nmaaga pa at wala pang masyadong impluwensya sila sayo.", "???"},
                            {"Nakilabutan ako sa sinabi niya. Andaming tanong ang nasa isip ko. Sinong sila?\nBakit kailangan maaga? at paano ako makakauwi?", "???"},
                            {"Kung ano-ano ang pumapasok sa utak ko. Pero, alam ko na ang pinaka kailangan\nkong gawin ay tanungin kung pano makauwi.", "???"},
                            {"'Pano po?' Mabilis kong tanong.", "???"},
                            {"Mukhang alanganin na sumagot yung Ale at tingin ng tingin sa mga katabi namin.\nDun ko napansin ang mga tingin nila. Parang mga madarangit ang tingin nila.\nNakakapangliit at nakakatakot. nararamdaman ko ang mga titig nila sa likod ko.\nKelan pa sila nakatitig? Kanina pa nang mapunta ako o  Gusto kong tumakbo.", "???"},
                            {"Takbo kaya ako? Pero saang direksyon? Kaliwa? Kanan? Kung tatakbo ba ko\nmakakabalik kaya ako? Pano? Marami silang nandito, ano gagawin ko", "???"},
                            {"Bigla hinawakan ni Ale yung kamay ko. Hinimas-himas nya ang palad ko at\nnararamdaman ko may sinusulat sya sa kamay ko. Yumuko ako para mabasa ko ang\nsinusulat nya.", "???"}, //IMAGE PATH REMEMBER
                            {"'Walong Lagusan'", "???"},
                            {"'Hanap Mapa'", "???"},
                            {"'Dalaketnon Wag sama'", "???"},
                            {"Iniintindi ko pa ang sinusulat nya ng hinatak sya ng isang matangkad babae.Tumingala\nako at nakita na ang nerbyos at takot ng Ale. Hawak-hawak nung babae ang wrist niya\nat pwersahang pinaharap niya si Ale sa kanya. Niyakap sya patalikod at sinubukan ni\nAle na lumayo pero masyadong malakas ang yakap nung babae at di maka-alis si Ale.", "???"},
                            {"Nak, mauuna na kami ng asawa ko. Malapit na magsimula ang pagdaraos, may mga\nkailangan pa kaming ayusin at alagaang mga bata. Mag-ingat ka ah, wag magpagutom.\nSabi niya.", "???"},
                            {"'Ale', Nerbyos kong sabi,", "???"},
                            {"Gusto ko tumulong. Gusto ko ilayo si Ale at halatang pinupwersa sya..", "???"},
                            {"'Okay ka lang ba?' Kailangan mo ba ng tulong’ gusto kong sabihin sa kanya ngunit\nparang alam nya na. Umiling sya at at kumaway. Tumingin ako sa isang babae at dun\nko nakita yung talas ng tingin nya sakin.Na akala mo’y sing tulis ng kutsilyo ang tingin.\nMas nakakapanindig balahibo pa ang kanyang mga titig kesa sa mga naramdaman ko", "???"},
                            {"ngayon. Gusto nya ko patayin. Galit sya. Halatang ayaw niya na may lumalapit o\nkumakausap dun kay Ale.", "???"},
                            {"Gusto ko gumalaw at umalis pero para akong naging estatwa na walang magawa\nkundi pagmasdan sila. Duon ko napansin ang mata nya. Baliktad ang nire-reflect ang\nimahe ko sa mga mata nya. Wala rin syang philtrum Tinignan ko ng mabilis ulit ang Ale\nat nakita ko sa kanyang mata na katulad yung sa kanya sa babae at pati sya ay wala", "???"},
                            {"ring philtrum.", "???"},
                            {"Huh. Maniwala pa rin ba ako sa sinabi niya? Kung parehas naman sila.", "???"},
                            {"Ngumiti at kumaway ulit si Ale at umalis na sila. Pero bago sya umalis ay binigkas\nang sa tingin ay ang salitang ‘Wag kang kumain’ at ngumiti ulit sya pero ngayon\nmay halong lungkot na. Na parang wala na syang magagawa pa. Napakaway na\nlang rin ako.", "???"},
                            {"Parang mas magandang mamatay, kung ganyan rin naman pala ang katumbas ng\npagpili mong manatili sa paraisong ganto. Paraiso nga ba talaga toh o isang\nhawla para sa mga naakit?", "???"},
                            {"Kailangan ko ng umuwi agad. ", "???"},
                            {"Andaming tanong ang pumapasok sa utak ko. Muli akong tumingin at nakita kong\npatapos na ang parada. Wala na rin ang tingin ng ibang mga tao sa amin.\nNi-try ko pakingan ang mga pag-uusap ng mga nakapaligid sakin. ", "???"},
                            {"Saan mo gusto pumunta? Alas dose pa dadating ang parada sa Madyas.", "???"},
                            {"Pede tayo dumaan muna sa Sanmalan at kunin yung mga inorder mo. Pede na rin\ntayo kumain dun love.", "???"},
                            {"Pede tas sunduin muna rin si Aye at yung mga kapatid mo sa may Mall", "???"},
                            {"Kain tayo Tapul", "???"},
                            {"Tawagan mo nga si Princess, kanina pa tayo nagi-intay.", "???"},
                            {"Naglakad-lakad ako at naghanap kung mga may nakapaskil na mapa. Napa-isip ako\nsa dati kong napanood sa KMJS. sang ultramodern metropolis na mga engkanto ang\nnakatira. Konti lang ang naalala ko sa episode na yan. Ang pinakana-alala ko ay isa\nitong siyudad ng engkanto at dahil sa mitong toh nakilala yung bayan na kinalakihan\nnila mama.", "???"},
                            {"Naalala ko yung mga warnings sa sinabi ng Ale. Wag ka kumain ng kung anong\nibigay nila. Wag kang tumanggap ng kahit ano at isipin mo muna kung ano ang\nsasabihin mo at pede nila gamitin yun sayo at sabihin na may tinanggap ka galing sa\nkanila.", "???"},
                            {"Parang tao lang rin sila pero mas makapangyarihan. Volatile sila at kahit anong spin\nang gawin para gawing romantiko, may malaking rason bakit binabalaan ang\nmga nakikinig.", "???"},
                            {"May nahanap na rin akong board na parang may nakalagay na mapa. Katabi nito\nay ang isang shed na parang sa bus stop. Maayos at mas magandang bus stop.", "???"},
                            {"Binilisan ko ang paglakad at tinignan ang nakapaskil.", "???"},
                            //PICTURE OF THE MAP HERE NEEDED REMEMBER THIS
                            {"Natawa na lang ako.", "???"},
                            {"PUTANGINA. Ba’t ba ako nag-expect na naka-tagalog o bisaya ang nakasulat dito.\nParang baybayin ang nakasulat pero iba ang estilo at alam ko iba na ang ibig sabihin\nng mga salita.", "???"},
                            {"Habang nakatulala ako nakatingin sa mapa, kinapa ko kung dala ko ba yung phone ko.\nAnd lo and behold, di ko dala. Iniwanan ko sa higaan ko yun na naka-charge kasi akala\nko dyan lang. ", "???"},
                            {"Takteng buhay nga naman. Wala pa kong dalang pencil o kung anong pedeng\nmakatulong at ang dala dala ko lang ay yung pagkain na pinadala ni lola na di ko alam\nkung pano ko hindi pa toh natapon.", "???"},
                            {"Mamatay kaya ako dito. Mamatay ba ako, mastuck ako dito habang buhay. Change\nmuna ng thoughts. ", "???"},
                            {"Tumingin ulit ako sa mapa, at ni-try ko itatak sya sa memorya ko. May apat silang\ndistrikto, nasa una ata akong distrikto. Di ko alam kung ano ang ibang simbolo, pero\niisipin ko base sa shape ay yung pabilog ay isa sa mga lagusan.", "???"},
                            {"Sa distrikto kung nasaan ako, may dalawang lagusan at mukhang yung sinasabing\nmalaking puno malapit sa city hall ang isa. ", "???"},
                            {"Pumunta ba ako sa isa o bumalik ulit dun at i-try bumalik?", "???"}
                            //CHOICES HERE


                        };
                        
                        dialogueBox.startDialogue(chapter2Dialogues);
                    }

                    if (!dialogueBox.isVisible()) {
                        showDialogue = false;
                    }

                    // Save progress
                    GameProgress currentProgress;
                    currentProgress.backgroundImage = bgManager.getCurrentBackground();
                    currentProgress.speaker = dialogueBox.getCurrentSpeaker();
                    currentProgress.dialogue = dialogueBox.getCurrentDialogue();
                    SaveManager::saveProgress("savegame.dat", currentProgress);
                }
                else {
                    sf::Text gameplayText;
                    gameplayText.setFont(bodyFont);
                    gameplayText.setString("Gameplay is running...\nPress ESC to return.");
                    gameplayText.setCharacterSize(30);
                    gameplayText.setFillColor(sf::Color::White);
                    gameplayText.setPosition(50, 200);
                    window.draw(gameplayText);
                }

                window.display();

                if (state == TitleScreen::GameState::TITLE)
                    break;
            }

            soundManager.stopMusic();
        }
        else if (state == TitleScreen::GameState::QUIT) {
            std::cout << "Quitting the game...\n";
            window.close();
        }
    }

    return 0;
}
