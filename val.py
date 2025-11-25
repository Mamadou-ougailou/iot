"""
val.py
Valide un fichier JSON produit par l'ESP32
selon un schéma défini à l’avance.
Utilise la bibliothèque 'jsonschema'.
"""

import json
from jsonschema import validate
from jsonschema.exceptions import ValidationError, SchemaError

# --- Définition du schéma attendu  ---
schema = {
    "type": "object",
    "properties": {
        "status": {
            "type": "object",
            "properties": {
                "temperature": {"type": "number"},
                "light": {"type": "number"},
                "regul": {"type": "string"},
                "fire": {"type": "boolean"},
                "heat": {"type": "string"},
                "cold": {"type": "string"},
                "fanspeed": {"type": "number"}
            },
            "required": ["temperature", "light", "regul", "fire"]
        },
        "location": {
            "type": "object",
            "properties": {
                "room": {"type": "string"},
                "gps": {
                    "type": "object",
                    "properties": {
                        "lat": {"type": "number"},
                        "lon": {"type": "number"}
                    },
                    "required": ["lat", "lon"]
                },
                "address": {"type": "string"}
            },
            "required": ["room", "gps"]
        },
        "regul": {
            "type": "object",
            "properties": {
                "lt": {"type": "number"},
                "ht": {"type": "number"}
            },
            "required": ["lt", "ht"]
        },
        "info": {
            "type": "object",
            "properties": {
                "ident": {"type": "string"},
                "user": {"type": "string"},
                "loc": {"type": "string"}
            },
            "required": ["ident", "user", "loc"]
        },
        "net": {
            "type": "object",
            "properties": {
                "uptime": {"type": "string"},
                "ssid": {"type": "string"},
                "mac": {"type": "string"},
                "ip": {"type": "string"}
            },
            "required": ["ssid", "ip"]
        },
        "reporthost": {
            "type": "object",
            "properties": {
                "target_ip": {"type": "string"},
                "target_port": {"type": "number"},
                "sp": {"type": "number"}
            },
            "required": ["target_ip", "target_port", "sp"]
        }
    },
    "required": ["status", "location", "regul", "info", "net", "reporthost"]
}

# --- Fonction principale ---
def validate_json_file(filename):
    try:
        with open(filename, "r", encoding="utf-8") as f:
            data = json.load(f)

        validate(instance=data, schema=schema)
        print("✅ Le fichier JSON est valide selon le schéma.")
    except FileNotFoundError:
        print(f"Fichier non trouvé : {filename}")
    except json.JSONDecodeError as e:
        print(f"Erreur de syntaxe JSON : {e}")
    except ValidationError as e:
        print(f"JSON invalide : {e.message}")
    except SchemaError as e:
        print(f"Erreur dans le schéma : {e.message}")

# --- Lancer la validation ---
if __name__ == "__main__":
    validate_json_file("test.json")
