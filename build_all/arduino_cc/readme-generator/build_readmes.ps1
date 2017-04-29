md -Force -Name built_readmes
md -Force -Name built_readmes/AzureIoTHub
md -Force -Name built_readmes/AzureIoTProtocol_HTTP
md -Force -Name built_readmes/AzureIoTProtocol_MQTT
md -Force -Name built_readmes/AzureIoTUtility

# Build the readme.md's for the exported libraries of make_sdk.cmd
Get-Content code_of_conduct_fragment.md, title_hub_fragment.md, contributor_not_here.md, main_body_fragment.md, license_fragment.md  | 
 % { $_ -replace “{{protocol_uc}}”,”HTTP” } |
 % { $_ -replace “{{protocol_lc}}”,”http” } |
Set-Content built_readmes/AzureIoTHub/readme.md

Get-Content code_of_conduct_fragment.md, title_http_fragment.md, contributor_not_here.md, main_body_fragment.md, license_fragment.md  | 
 % { $_ -replace “{{protocol_uc}}”,”HTTP” } |
 % { $_ -replace “{{protocol_lc}}”,”http” } |
Set-Content built_readmes/AzureIoTProtocol_HTTP/readme.md

Get-Content code_of_conduct_fragment.md, title_mqtt_fragment.md, contributor_not_here.md, main_body_fragment.md, license_fragment.md  | 
 % { $_ -replace “{{protocol_uc}}”,”MQTT” } |
 % { $_ -replace “{{protocol_lc}}”,”mqtt” } |
Set-Content built_readmes/AzureIoTProtocol_MQTT/readme.md

Get-Content code_of_conduct_fragment.md, title_utility_fragment.md, contributor_not_here.md, main_body_fragment.md, license_fragment.md  | 
 % { $_ -replace “{{protocol_uc}}”,”HTTP” } |
 % { $_ -replace “{{protocol_lc}}”,”http” } |
Set-Content built_readmes/AzureIoTUtility/readme.md



# Build the readme.md's for the source tree
Get-Content code_of_conduct_fragment.md, title_hub_fragment.md, main_body_fragment.md, contributor_yes.md, license_fragment.md  | 
 % { $_ -replace “{{protocol_uc}}”,”HTTP” } |
 % { $_ -replace “{{protocol_lc}}”,”http” } |
Set-Content ../base-libraries/AzureIoTHub/readme.md

Get-Content code_of_conduct_fragment.md, title_http_fragment.md, main_body_fragment.md, contributor_yes.md, license_fragment.md  | 
 % { $_ -replace “{{protocol_uc}}”,”HTTP” } |
 % { $_ -replace “{{protocol_lc}}”,”http” } |
Set-Content ../base-libraries/AzureIoTProtocol_HTTP/readme.md

Get-Content code_of_conduct_fragment.md, title_mqtt_fragment.md, main_body_fragment.md, contributor_yes.md, license_fragment.md  | 
 % { $_ -replace “{{protocol_uc}}”,”MQTT” } |
 % { $_ -replace “{{protocol_lc}}”,”mqtt” } |
Set-Content ../base-libraries/AzureIoTProtocol_MQTT/readme.md

Get-Content code_of_conduct_fragment.md, title_utility_fragment.md, main_body_fragment.md, contributor_yes.md, license_fragment.md  | 
 % { $_ -replace “{{protocol_uc}}”,”HTTP” } |
 % { $_ -replace “{{protocol_lc}}”,”http” } |
Set-Content ../base-libraries/AzureIoTUtility/readme.md


