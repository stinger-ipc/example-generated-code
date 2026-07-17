#[allow(unused_imports)]
use crate::payloads::*;
use derive_builder::Builder;

#[derive(Clone, Builder, Debug)]
pub struct PropOnlyInitialPropertyValues {
    pub home_address: Address,
    pub home_address_version: u32,

    pub favorite_country: Country,
    pub favorite_country_version: u32,
}
