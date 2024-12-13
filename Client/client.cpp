#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include "json.hpp"

using json = nlohmann::json;

std::string server = "";
std::string endpoint = "/cars/";
std::string server_and_endpoint = "";

//these arguments are being used in main() to decide which action to call
const std::string read_all_argument = "-read_all";
const std::string create_car_argument = "-create_car";
const std::string read_car_argument = "-read_car";
const std::string update_car_argument = "-update_car";
const std::string delete_car_argument = "-delete_car";

//the functions are self explanatory by their function names
void read_all()
{
    std::cout << "Retrieving all cars, please wait" << std::endl;
    std::cout << "-----------" << std::endl;

    auto response = cpr::Get(cpr::Url{ server_and_endpoint });

    json response_data = json::parse(response.text);

    for (auto& [key, value] : response_data.items())
    {
        std::cout << "ID: " << value["id"] << std::endl;
        std::cout << "Vendor: " << value["vendor"] << std::endl;
        std::cout << "Model: " << value["model"] << std::endl;
        std::cout << "Color: " << value["color"] << std::endl;

        std::cout << "-----------" << std::endl;
    }
}

void create_car(std::string vendor, std::string model, std::string color)
{
    json create_car_json = {
        {"vendor", vendor},
        {"model", model},
        {"color", color}
    };

    std::cout << "Creating car. " << "Vendor: " << vendor << ", " << "Model: " << model << ", " << "Color: " << color << std::endl;

    auto response = cpr::Post(cpr::Url{ server_and_endpoint },
        cpr::Body{ create_car_json.dump() /* converts it to a string */ },
        cpr::Header{ { "Content-Type", "application/json" } });

    if (response.status_code == 200)
    {
        std::cout << "Car successfully created" << std::endl;

        json json_response = json::parse(response.text);
        std::cout << "ID of the new car: " << json_response["id"] << std::endl;
    }
    else
    {
        std::cout << "Error, see returned status code" << std::endl;
        std::cout << "Returned Status Code: " << response.status_code << std::endl;
    }
}

void read_car(std::string ID)
{
    std::cout << "Retrieving car with the ID " << ID << std::endl;
    std::cout << "-----------" << std::endl;

    auto response = cpr::Get(cpr::Url{ server_and_endpoint + ID });
    
    if (response.status_code == 404)
        std::cout << "No car with such an ID found" << std::endl;
    else
    {
        json response_data = json::parse(response.text);

        std::cout << "ID: " << response_data["id"] << std::endl;
        std::cout << "Vendor: " << response_data["vendor"] << std::endl;
        std::cout << "Model: " << response_data["model"] << std::endl;
        std::cout << "Color: " << response_data["color"] << std::endl;

        std::cout << "-----------" << std::endl;
    }
}

void update_car(std::string ID, std::string color)
{
    json create_car_color_json = {
    {"color", color}
    };

    std::cout << "Updating color of the car with the ID " << ID << std::endl;

    auto response = cpr::Patch(cpr::Url{ server_and_endpoint + ID },
        cpr::Body{ create_car_color_json.dump() },
        cpr::Header{ { "Content-Type", "application/json" } });

    if (response.status_code == 200)
        std::cout << "Car with the ID " << ID << " " << "successfully updated to " << color << std::endl;
    else
    {
        if (response.status_code == 404)
            std::cout << "No car with such an ID found" << std::endl;
        else
        {
            std::cout << "Error, see returned status code" << std::endl;
            std::cout << "Returned Status Code: " << response.status_code << std::endl;
        }
    }
}

void delete_car(std::string ID)
{
    std::cout << "Deleting car with the ID " << ID << std::endl;

    auto response = cpr::Delete(cpr::Url{ server_and_endpoint + ID },
        cpr::Header{ { "Content-Type", "application/json" } });

    if (response.status_code == 200)
        std::cout << "Car with the ID " << ID << " " << "successfully deleted" << std::endl;
    else
    {
        if (response.status_code == 404)
            std::cout << "No car with such an ID found" << std::endl;
        else
        {
            std::cout << "Error, see returned status code" << std::endl;
            std::cout << "Returned Status Code: " << response.status_code << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        server = argv[1];
        server_and_endpoint = server + endpoint;
    }
    else
        std::cout << "Not enough arguments" << std::endl;

    if (argc == 3 && argv[2] == read_all_argument)
        read_all();

    if (argc == 6 && argv[2] == create_car_argument)
        create_car(argv[3], argv[4], argv[5]);

    if (argc == 4 && argv[2] == read_car_argument)
        read_car(argv[3]);

    if (argc == 5 && argv[2] == update_car_argument)
        update_car(argv[3], argv[4]);

    if (argc == 4 && argv[2] == delete_car_argument)
        delete_car(argv[3]);

    return 0;
}