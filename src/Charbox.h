#ifndef CHARBOX_H
#define CHARBOX_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <map>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Charbox {
private:
    std::string api_base = "https://charbox.org/api/v2";
    http_client_config client_config;
    
    http_request create_request(const std::string& path, method m, const std::string& body_data = "", const std::string& content_type = "") {
        http_request request(m);
        
        // Полный набор браузерных заголовков
        request.headers().add("content-length", 400); 
        request.headers().add("Content-Type", "application/json"); 
        request.headers().add("host", "charbox.org"); 
        request.headers().add("referer", "ahttps://charbox.org/");
        request.headers().add("origin", "https://charbox.org");
        // Устанавливаем тело запроса, если передано
        if (!body_data.empty() && !content_type.empty()) {
            request.set_body(body_data, utility::conversions::to_string_t(content_type));
        }
        
        request.set_request_uri(utility::conversions::to_string_t(path));
        return request;
    }

public:
    Charbox() {
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> get_character(const std::string& class_str,const std::string& class_archetype,const std::string& race,const std::string& race_type,const std::string& background,int level,const std::string& ability_points_type,const json::value& traits_list,int stats_up_count,const std::string& hits_type ,const std::string& gender) {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        json::value data;
        data[U("class")] = json::value(utility::conversions::to_string_t(class_str));
        data[U("class_archetype")] = json::value(utility::conversions::to_string_t(class_archetype));
        data[U("race")] = json::value(utility::conversions::to_string_t(race));
        data[U("race_type")] = json::value(utility::conversions::to_string_t(race_type));
        data[U("background")] = json::value(utility::conversions::to_string_t(background));
        data[U("level")] = json::value::number(level);
        data[U("ability_points_type")] = json::value(utility::conversions::to_string_t(ability_points_type));
        data[U("traits_list")] = traits_list;
        data[U("stats_up_count")] = json::value::number(stats_up_count);
        data[U("hits_type")] = json::value(utility::conversions::to_string_t(hits_type));
        data[U("gender")] = json::value(utility::conversions::to_string_t(gender));
        std::string path = "/get-character";
        auto request = create_request(path, methods::POST, data.serialize(),"application/json");
        
        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: " + std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: " + std::string(e.what())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }
};

#endif
