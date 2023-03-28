#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <vector>
#include <vector_can.hpp>

using BrsController = std::string;

const BrsController AMC = "AMC";
const BrsController MBC = "MBC";
const BrsController TMC = "TMC";

std::vector<BrsController> controllers = {
    AMC, MBC, TMC
};

nana::form* form = nullptr;
nana::combox* ctrlComBox = nullptr;

void OnSelectFileBtnClick()
{
    std::cout << "Select File Button was clicked" << std::endl;

    nana::filebox binFileBox(*form, true);
    binFileBox.add_filter(("*.bin"), ("*.bin"));
    binFileBox.add_filter(("*"), ("*"));
    auto files = binFileBox();
    if (!files.empty())
    {
        std::cout << "Selected file:  " << files.front().string() << std::endl;
    }
}

void FlashOverCan(const BrsController& ctrl)
{
    if (!vector::InitDriver())
        throw new std::runtime_error("Failed to initialize Vector Xl Driver");
    std::cout << "Vector Xl Driver initialized" << std::endl;

    vector::App app("BrsCanFlasher", XL_HWTYPE_VN1610);
    std::cout << "Vector application created" << std::endl;

    vector::Port port = app.OpenPort(256);
    port.SetTimerRate(10);

    if (!vector::CloseDriver())
        throw new std::runtime_error("Failed to close Vector Xl Driver");
    std::cout << "Vector Xl Driver closed" << std::endl;
}

void OnFlashFirmwareBtnClick()
{
    std::cout << "Flash Firmware Button was clicked" << std::endl;

    const size_t ctrlIdx = ctrlComBox->option();
    if (ctrlIdx >= controllers.size())
    {
        std::cout << "No controller selected" << std::endl;
        return;
    }

    const BrsController ctrl = controllers[ctrlIdx];
    std::cout << ctrl << " selected" << std::endl;

    if (ctrl == AMC)
    {

    }
    else if (ctrl == MBC)
    {

    }
    else if (ctrl == TMC)
    {

    }
    else
    {
        throw new std::runtime_error("Selected controller not implemented");
    }

    FlashOverCan(ctrl);
}

int main()
{
    std::cout << "Starting BrsCanFlasher" << std::endl;

    form = new nana::form();
    form->caption("BrsCanFlasher");
    
    nana::button selectFileBtn(*form, nana::rectangle(20, 20, 150, 30));
    selectFileBtn.caption("Select .bin File");
    selectFileBtn.events().click(OnSelectFileBtnClick);

    ctrlComBox = new nana::combox(*form, nana::rectangle(20, 60, 150, 30));
    ctrlComBox->caption("Select Controller");
    for (const BrsController& ctrl : controllers)
        ctrlComBox->push_back(ctrl);

    nana::button flashFwBtn(*form, nana::rectangle(20, 100, 150, 30));
    flashFwBtn.caption("Flash Firmware");
    flashFwBtn.events().click(OnFlashFirmwareBtnClick);

    form->show();
    nana::exec();

    std::cout << "Ending BrsCanFlasher" << std::endl;
    delete form;
    delete ctrlComBox;
    return 0;
}
